/*
 * sample_timer.c
 *
 *  Created on: 2023年5月29日
 *      Author: gy
 */
#include "sample_timer.h"
#include "PID.h"
#include "leds.h"

//int interrupt_count;
//定时器2初始化函数
//Freq：CPU时钟频率（150MHz）
//Period：定时周期值，单位us

void TIM2_Init(float Freq, float Period)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 1; // CPU Timer 2
    EDIS;

    //设置定时器2的中断入口地址为中断向量表的TINT2
    EALLOW;
    PieVectTable.TINT2 = &TIM2_IRQn;
    EDIS;

    //指向定时器2的寄存器地址
    CpuTimer2.RegsAddr = &CpuTimer2Regs;
    //设置定时器2的周期寄存器值
    CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;
    //设置定时器预定标计数器值为0
    CpuTimer2Regs.TPR.all  = 0;
    CpuTimer2Regs.TPRH.all = 0;
    //确保定时器2为停止状态
    CpuTimer2Regs.TCR.bit.TSS = 1;
    //重载使能
    CpuTimer2Regs.TCR.bit.TRB = 1;
    // Reset interrupt counters:
    CpuTimer2.InterruptCount = 0;

    ConfigCpuTimer(&CpuTimer2, Freq, Period);

    //开始定时器功能
    CpuTimer2Regs.TCR.bit.TSS=0;
    //定时器2直接连到内核中断，无需配置PIE
    IER |= M_INT14;

    //使能总中断
    EINT;
    ERTM;

}

interrupt void TIM2_IRQn(void)
{
    EALLOW;
    LED2_TOGGLE;
 //   ConfigCpuTimer(&CpuTimer2, 150, sampling_T);
//    interrupt_count++;
//    if (interrupt_count>60000)
//    {
//        interrupt_count=0;
//    }

    Get_speed(&speed_pid, &qep_posspeed);
    PID_Calc(&speed_pid);


    if (va + speed_pid.OUT<15000)//限制最大占空比
    {
        va += speed_pid.OUT;
    }
//    else
//    {
//        va = 14999;
//    }
    CpuTimer2Regs.TCR.bit.TSS=0;//启动定时器
    EDIS;
}
