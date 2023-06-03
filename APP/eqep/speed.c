/*
 * speed.h
 *
 *  Created on: 2023年4月16日
 *      Author: gy
 */


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"     // Device Headerfile and Examples Include File
#include "speed.h"   // Example specific Include file

POSSPEED qep_posspeed=POSSPEED_DEFAULTS;

void  POSSPEED_Init(void)
{

    #if (CPU_FRQ_150MHZ)
      EQep1Regs.QUPRD=1500000;          // Unit Timer for 100Hz at 150 MHz SYSCLKOUT单位时间计数器计数周期（计数值）0.01s        （此寄存器包含用于生成周期单位时间事件的单位计时器的周期计数。这些事件以周期性间隔锁定eQEP位置信息）
    #endif
    #if (CPU_FRQ_100MHZ)
      EQep1Regs.QUPRD=1000000;          // Unit Timer for 100Hz at 100 MHz SYSCLKOUT
    #endif

    EQep1Regs.QDECCTL.bit.QSRC=00;      // QEP quadrature count mode 正交计数模式

    EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
    EQep1Regs.QEPCTL.bit.PCRM=10;       // PCRM=10 mode - QPOSCNT reset on first index event      QPOSCNT(位置计数器)的时钟来源于QCLK
    EQep1Regs.QEPCTL.bit.UTE=1;         // Unit Timeout Enable
    EQep1Regs.QEPCTL.bit.QCLM=1;        // Latch on unit time out   单位时间事件发生时锁存数据。位置计数器值、捕获计时器和捕获周期值保存在QPOSLAT、QCTMRLAT和QCPRDLAT寄存器中（保证捕获到的值是在同一个时间基准下）
    EQep1Regs.QPOSMAX=pulse_count*10;       //位置计数器的最大值。可以根据电机一圈的脉冲数确定.这里10圈清零一次
    EQep1Regs.QEPCTL.bit.QPEN=1;        // QEP enable

    EQep1Regs.QCAPCTL.bit.UPPS=1;       // 1/2 for unit position  2次QCLK产生一个UPEVENT脉冲
    EQep1Regs.QCAPCTL.bit.CCPS=7;       // 1/128 for CAP clock
    EQep1Regs.QCAPCTL.bit.CEN=1;        // QEP Capture Enable
    //EQep1Regs.QEPCTL.bit.IEI=2;         //索引信号上升沿时初始化锁存位置计数器
    //EQep1Regs.QEPCTL.bit.SWI=1;//软件复位 eQEP 子系统的命令,产生一次索引脉冲来标记旋转位置或进行校准
}

void POSSPEED_Calc(POSSPEED *p)
{

     unsigned int newp,oldp;//用于M法计算速度的中间量
     //int temp1;//用于T法计算速度的中间量

     p->DirectionQep = EQep1Regs.QEPSTS.bit.QDF;    //方向

// Check an index occurrence
     if (EQep1Regs.QFLG.bit.IEL == 1)   //索引事件中断
     {
        p->index_sync_flag = 0x00F0;
        EQep1Regs.QCLR.bit.IEL=1;                   // Clear interrupt flag
     }


    if(EQep1Regs.QFLG.bit.UTO==1)                    // If unit timeout (one 100Hz period) T已知（0.03s），▲X = QPOSLAT（k）-QPOSLAT（k-1）
    {

        newp=(unsigned int)EQep1Regs.QPOSLAT;
        oldp=p->oldpos;

        if (p->DirectionQep==0)                     // POSCNT is counting down，Tmp为负数
        {
            if (newp>oldp)
                p->Pos_increment = -EQep1Regs.QPOSMAX + newp - oldp;
            else
                p->Pos_increment = newp -oldp;
        }
        else if (p->DirectionQep==1)                // POSCNT is counting up
        {
            if (newp>=oldp)
                p->Pos_increment = newp - oldp;
            else
                p->Pos_increment = EQep1Regs.QPOSMAX - oldp + newp;                     // x2-x1 should be positive
        }
        p->Speed_fr = p->Pos_increment*p->BaseRpm;              //      r/min
        //_IQ15mpy(10000,p->BaseRpm_Q)是先将10000和p->BaseRpm_Q转化为long型，然后令两者相乘再除以2^15 (long最大值 4294967295)
        p->SpeedQ_fr= _IQ15mpy(p->Pos_increment*100,p->BaseRpm_Q); // 定点运算,Q15*Q0。这里得到真实值的100倍，防止小数点被省略,经测试，将p->Pos_increment*100中100改为10时，p->SpeedQ_fr的值时常出现跳变，难以用于pid调节
        // Update the electrical angle
        p->oldpos = newp;
        EQep1Regs.QCLR.bit.UTO=1;                   // Clear interrupt flag
    }

//**** Low-speed computation using QEP capture counter ****//
    //UPEVNT是新值被锁存到QCPRD寄存器的标志，即单位位置事件
    //单位位事件之间计数不能超过65535次
    //单位位置事件之间无方向变化
    if(EQep1Regs.QEPSTS.bit.UPEVNT==1)                 // Unit position event 32次QCLK，8个编码器脉冲周期.    X已知（UPPS控制），▲T锁存在捕获周期寄存器QCPRDLAT
    {
        if(EQep1Regs.QEPSTS.bit.COEF==0)               // No Capture overflow 没有溢出
            p->temp1=(unsigned long)EQep1Regs.QCPRDLAT;   // temp1 = t2-t1
        else                                           // Capture overflow, saturate the result
            p->temp1=0xFFFF;
        if(EQep1Regs.QEPSTS.bit.CDEF==1)//没有方向变化
            p->Speed_pr = p->SpeedScaler/p->temp1;    // p->Speed_pr = p->SpeedScaler/temp1  输出IQ24 实现过程是在函数内现将p->SpeedScaler,temp1转化为long型，然后相除得到为IQ24
        else
            p->Speed_pr = p->Speed_pr;
        EQep1Regs.QEPSTS.all=0x88;                  // Clear Unit position event flag
                                                    // Clear overflow error flag
    }


}

