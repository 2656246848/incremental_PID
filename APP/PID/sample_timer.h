/*
 * sample_timer.h
 *
 *  Created on: 2023��5��29��
 *      Author: gy
 */

#ifndef APP_PID_SAMPLE_TIMER_H_
#define APP_PID_SAMPLE_TIMER_H_


#include "DSP2833x_Device.h"     // DSP2833x ͷ�ļ�
#include "DSP2833x_Examples.h"   // DSP2833x �������ͷ�ļ�
#include "PID.h"

extern Uint16 va;
extern PID speed_pid;

void TIM2_Init(float Freq, float Period);
interrupt void TIM2_IRQn(void);

#endif /* APP_PID_SAMPLE_TIMER_H_ */
