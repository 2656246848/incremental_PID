/*
 * main.h
 *  Created on: 2023Äê5ÔÂ29ÈÕ
 *  Author: gy
 */

#ifndef USER_MAIN_H_
#define USER_MAIN_H_

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "leds.h"
#include "time.h"
#include "sample_timer.h"
#include "oled.h"
#include "exti.h"
#include "stdio.h"
#include "string.h"
#include "eqep.h"
#include "speed.h"
#include "uart.h"
#include "PID.h"

extern POSSPEED qep_posspeed;
extern PID speed_pid;

#define FLASH_RUN 1
#define SRAM_RUN 2
#define RUN_TYPE SRAM_RUN

#endif /* USER_MAIN_H_ */
