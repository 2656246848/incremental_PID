/*
 * led.c
 *
 *  Created on: 2022��9��8��
 *      Author: Administrator
 */

#include "leds.h"

void LED_Init(void)
{
       EALLOW;//�ر�д����

       SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    // ����GPIOʱ��

       //LED1�˿�����
       GpioCtrlRegs.GPCMUX1.bit.GPIO68=0;//����Ϊͨ��GPIO����
       GpioCtrlRegs.GPCDIR.bit.GPIO68=1;//����GPIO����Ϊ���
       GpioCtrlRegs.GPCPUD.bit.GPIO68=0;//ʹ��GPIO��������
       GpioDataRegs.GPCSET.bit.GPIO68=1;//����GPIO����ߵ�ƽ
       //LED2�˿�����
      GpioCtrlRegs.GPCMUX1.bit.GPIO67=0;//����Ϊͨ��GPIO����
      GpioCtrlRegs.GPCDIR.bit.GPIO67=1;//����GPIO����Ϊ���
      GpioCtrlRegs.GPCPUD.bit.GPIO67=0;//ʹ��GPIO��������
      GpioDataRegs.GPCSET.bit.GPIO67=1;//����GPIO����ߵ�ƽ

       EDIS;//����д����
}

