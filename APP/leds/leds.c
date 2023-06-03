/*
 * led.c
 *
 *  Created on: 2022年9月8日
 *      Author: Administrator
 */

#include "leds.h"

void LED_Init(void)
{
       EALLOW;//关闭写保护

       SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    // 开启GPIO时钟

       //LED1端口配置
       GpioCtrlRegs.GPCMUX1.bit.GPIO68=0;//设置为通用GPIO功能
       GpioCtrlRegs.GPCDIR.bit.GPIO68=1;//设置GPIO方向为输出
       GpioCtrlRegs.GPCPUD.bit.GPIO68=0;//使能GPIO上拉电阻
       GpioDataRegs.GPCSET.bit.GPIO68=1;//设置GPIO输出高电平
       //LED2端口配置
      GpioCtrlRegs.GPCMUX1.bit.GPIO67=0;//设置为通用GPIO功能
      GpioCtrlRegs.GPCDIR.bit.GPIO67=1;//设置GPIO方向为输出
      GpioCtrlRegs.GPCPUD.bit.GPIO67=0;//使能GPIO上拉电阻
      GpioDataRegs.GPCSET.bit.GPIO67=1;//设置GPIO输出高电平

       EDIS;//开启写保护
}

