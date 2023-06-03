/*
 * eqep.h
 *
 *  Created on: 2023Äê4ÔÂ16ÈÕ
 *      Author: gy
 */

#ifndef EQEP_H_
#define EQEP_H_

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"
#include "speed.h"

extern POSSPEED qep_posspeed;

void EQEP1_Init(void);
void EPwm1Setup(void);
void EPwm1A_SetCompare(Uint16 val);
void EPwm1B_SetCompare(Uint16 val);

#endif /* APP_EQEP_EQEP_H_ */
