/*
 * time.c
 *
 *  Created on: 2018-1-24
 *      Author: Administrator
 */


#include "time.h"
#include "leds.h"

//定时器1初始化函数
//Freq：CPU时钟频率（150MHz）
//Period：定时周期值，单位us
void TIM1_Init(float Freq, float Period)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; // CPU Timer 1
    EDIS;

    //设置定时器1的中断入口地址为中断向量表的INT13
    EALLOW;
    PieVectTable.XINT13 = &TIM1_IRQn;
    EDIS;

    //指向定时器1的寄存器地址
    CpuTimer1.RegsAddr = &CpuTimer1Regs;
    //设置定时器1的周期寄存器值
    CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
    //设置定时器预定标计数器值为0
    CpuTimer1Regs.TPR.all  = 0;
    CpuTimer1Regs.TPRH.all = 0;
    //确保定时器1为停止状态
    CpuTimer1Regs.TCR.bit.TSS = 1;
    //重载使能
    CpuTimer1Regs.TCR.bit.TRB = 1;
    // Reset interrupt counters:
    CpuTimer1.InterruptCount = 0;

    ConfigCpuTimer(&CpuTimer1, Freq, Period);

    //开始定时器功能
    CpuTimer1Regs.TCR.bit.TSS=0;
    //定时器1直接连到内核中断，无需配置PIE
    IER |= M_INT13;

    //使能总中断
    EINT;
    ERTM;

}

interrupt void TIM1_IRQn(void)
{
    EALLOW;
    LED1_TOGGLE;
    ConfigCpuTimer(&CpuTimer1, 150, 6.6858);
    CpuTimer1Regs.TCR.bit.TSS=0;

    EDIS;
}


