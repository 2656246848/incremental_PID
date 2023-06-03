/*
 * exti.h
 *
 *  Created on: 2023��4��14��
 *      Author: gy
 */
#include "exti.h"
#include "oled.h"

interrupt void EXTI3_IRQn(void);
interrupt void EXTI4_IRQn(void);
void EXTI_Init(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1; // GPIO input clock
    EDIS;
    EALLOW;
    //KEY1 �˿�����
    GpioCtrlRegs.GPBMUX2.bit.GPIO48=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO48=0;
    GpioCtrlRegs.GPBPUD.bit.GPIO48=0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO48 = 0; // �ⲿ�ж� 1  ,��XINT1����ϵͳʱ�� SYSCLKOUT ͬ��
    //KEY2 �˿�����
    GpioCtrlRegs.GPBMUX2.bit.GPIO49=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO49=0;
    GpioCtrlRegs.GPBPUD.bit.GPIO49=0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO49 = 0; // �ⲿ�ж� 2  ,��XINT1����ϵͳʱ�� SYSCLKOUT ͬ��

    GpioCtrlRegs.GPAMUX1.bit.GPIO12=0;
    GpioCtrlRegs.GPADIR.bit.GPIO12=1;
    GpioCtrlRegs.GPAPUD.bit.GPIO12=0;
    GpioDataRegs.GPACLEAR.bit.GPIO12=1;
    EDIS;
    EALLOW;
    GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = 48; // XINT1 �� GPIO12
    GpioIntRegs.GPIOXINT4SEL.bit.GPIOSEL = 49; // XINT2 �� GPIO13
    EDIS;
    EALLOW; // �޸ı������ļĴ������޸�ǰӦ��� EALLOW ���
    PieVectTable.XINT3 = &EXTI3_IRQn;
    PieVectTable.XINT4 = &EXTI4_IRQn;
    EDIS; // EDIS ����˼�ǲ������޸ı������ļĴ���
    PieCtrlRegs.PIEIER12.bit.INTx2 = 1; // ʹ�� PIE �� 12 ��INT2
    XIntruptRegs.XINT3CR.bit.POLARITY = 0; // �½��ش����ж�
    XIntruptRegs.XINT3CR.bit.ENABLE= 1; // ʹ�� XINT1
    PieCtrlRegs.PIEIER12.bit.INTx1 = 1; // ʹ�� PIE �� 12 ��INT1
    XIntruptRegs.XINT4CR.bit.POLARITY = 0; // �½��ش����ж�
    XIntruptRegs.XINT4CR.bit.ENABLE= 1; // ʹ�� XINT2
    IER |= M_INT12; // ʹ�� CPU �ж� ��1    ��INT1��
    EINT; // ��ȫ���ж�
    ERTM;
}

interrupt void EXTI3_IRQn(void)
{
    DELAY_US(100000); //����������
    while(GpioDataRegs.GPBDAT.bit.GPIO48);
    if(va<=90*150)
    {
        va+=1500;
        OLED_ShowNum(60,2,va,2,2,16);
    }
    else
    {
       va=0;
       OLED_ShowNum(60,2,va,2,2,16);
    }

    LED1_TOGGLE;
    PieCtrlRegs.PIEACK.bit.ACK1=1;
}

interrupt void EXTI4_IRQn(void)
{
    DELAY_US(100000); //����������
    while(GpioDataRegs.GPBDAT.bit.GPIO49);
    if(va<=15000)
    {
        va+=150;
        OLED_ShowNum(60,2,va,2,2,16);
    }
    else
    {
       va=0;
       OLED_ShowNum(60,2,va,2,2,16);
    }

    LED2_TOGGLE;
    PieCtrlRegs.PIEACK.bit.ACK1=1;
}
