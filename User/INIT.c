/*
 * INIT.c
 *
 *  Created on: 2023年5月29日
 *      Author: gy
 */

#include "main.h"

void SYSTERM_INIT()
{
    InitSysCtrl();
    //初始化PIE控制寄存器和PIE中断向量表
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    LED_Init();
    EXTI_Init();
    OLED_Init();
    OLED_Clear();
    EQEP1_Init();
    UARTa_Init(9600);
    TIM2_Init(150,sampling_T);
    PID_Init(&speed_pid);
}
