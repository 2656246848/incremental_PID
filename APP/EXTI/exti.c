/*
 * exti.h
 *
 *  Created on: 2023年4月14日
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
    //KEY1 端口配置
    GpioCtrlRegs.GPBMUX2.bit.GPIO48=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO48=0;
    GpioCtrlRegs.GPBPUD.bit.GPIO48=0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO48 = 0; // 外部中断 1  ,（XINT1）与系统时钟 SYSCLKOUT 同步
    //KEY2 端口配置
    GpioCtrlRegs.GPBMUX2.bit.GPIO49=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO49=0;
    GpioCtrlRegs.GPBPUD.bit.GPIO49=0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO49 = 0; // 外部中断 2  ,（XINT1）与系统时钟 SYSCLKOUT 同步

    GpioCtrlRegs.GPAMUX1.bit.GPIO12=0;
    GpioCtrlRegs.GPADIR.bit.GPIO12=1;
    GpioCtrlRegs.GPAPUD.bit.GPIO12=0;
    GpioDataRegs.GPACLEAR.bit.GPIO12=1;
    EDIS;
    EALLOW;
    GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = 48; // XINT1 是 GPIO12
    GpioIntRegs.GPIOXINT4SEL.bit.GPIOSEL = 49; // XINT2 是 GPIO13
    EDIS;
    EALLOW; // 修改被保护的寄存器，修改前应添加 EALLOW 语句
    PieVectTable.XINT3 = &EXTI3_IRQn;
    PieVectTable.XINT4 = &EXTI4_IRQn;
    EDIS; // EDIS 的意思是不允许修改被保护的寄存器
    PieCtrlRegs.PIEIER12.bit.INTx2 = 1; // 使能 PIE 组 12 的INT2
    XIntruptRegs.XINT3CR.bit.POLARITY = 0; // 下降沿触发中断
    XIntruptRegs.XINT3CR.bit.ENABLE= 1; // 使能 XINT1
    PieCtrlRegs.PIEIER12.bit.INTx1 = 1; // 使能 PIE 组 12 的INT1
    XIntruptRegs.XINT4CR.bit.POLARITY = 0; // 下降沿触发中断
    XIntruptRegs.XINT4CR.bit.ENABLE= 1; // 使能 XINT2
    IER |= M_INT12; // 使能 CPU 中断 组1    （INT1）
    EINT; // 开全局中断
    ERTM;
}

interrupt void EXTI3_IRQn(void)
{
    DELAY_US(100000); //键盘消抖动
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
    DELAY_US(100000); //键盘消抖动
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
