/*
 * INIT.c
 *
 *  Created on: 2023��5��29��
 *      Author: gy
 */

#include "main.h"

void SYSTERM_INIT()
{
    InitSysCtrl();
    //��ʼ��PIE���ƼĴ�����PIE�ж�������
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
