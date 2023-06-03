/*
 * time.c
 *
 *  Created on: 2018-1-24
 *      Author: Administrator
 */


#include "time.h"
#include "leds.h"

//��ʱ��1��ʼ������
//Freq��CPUʱ��Ƶ�ʣ�150MHz��
//Period����ʱ����ֵ����λus
void TIM1_Init(float Freq, float Period)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 1; // CPU Timer 1
    EDIS;

    //���ö�ʱ��1���ж���ڵ�ַΪ�ж��������INT13
    EALLOW;
    PieVectTable.XINT13 = &TIM1_IRQn;
    EDIS;

    //ָ��ʱ��1�ļĴ�����ַ
    CpuTimer1.RegsAddr = &CpuTimer1Regs;
    //���ö�ʱ��1�����ڼĴ���ֵ
    CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
    //���ö�ʱ��Ԥ���������ֵΪ0
    CpuTimer1Regs.TPR.all  = 0;
    CpuTimer1Regs.TPRH.all = 0;
    //ȷ����ʱ��1Ϊֹͣ״̬
    CpuTimer1Regs.TCR.bit.TSS = 1;
    //����ʹ��
    CpuTimer1Regs.TCR.bit.TRB = 1;
    // Reset interrupt counters:
    CpuTimer1.InterruptCount = 0;

    ConfigCpuTimer(&CpuTimer1, Freq, Period);

    //��ʼ��ʱ������
    CpuTimer1Regs.TCR.bit.TSS=0;
    //��ʱ��1ֱ�������ں��жϣ���������PIE
    IER |= M_INT13;

    //ʹ�����ж�
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


