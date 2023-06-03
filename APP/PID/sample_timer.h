/*
 * sample_timer.h
 *
 *  Created on: 2023年5月29日
 *      Author: gy
 */

#ifndef APP_PID_SAMPLE_TIMER_H_
#define APP_PID_SAMPLE_TIMER_H_


#include "DSP2833x_Device.h"     // DSP2833x 头文件
#include "DSP2833x_Examples.h"   // DSP2833x 例子相关头文件
#include "PID.h"

extern Uint16 va;
extern PID speed_pid;

void TIM2_Init(float Freq, float Period);
interrupt void TIM2_IRQn(void);

#endif /* APP_PID_SAMPLE_TIMER_H_ */
