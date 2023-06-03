/*
 * PID.h
 *
 *  Created on: 2023年5月29日
 *      Author: gy
 */

#ifndef PID_H_
#define PID_H_

#include "eqep.h"
#include "speed.h"
extern POSSPEED qep_posspeed;//获取速度信息


/*PID参数相关宏*/
#define     P               1.0f        //2.5
#define     I               0.0f        //0
#define     D               0.0f        //0
#define     sampling_T      12000      //采样周期20ms（12000us）

/*PID结构体*/
typedef struct
{
    float sv;//设定值
    float pv;//实际值
    float Kp;
    float Kd;
    float Ki;
    float EK;//本次偏差
    float Last_EK;//上次偏差
    float prev_EK;//上次偏差
    float OUT;
    float system_scale;

}PID;



void PID_Init();
float Get_speed(PID *p,POSSPEED *p2);
//float Get_Average_Speed(PID *p, POSSPEED *p2);
void PID_Calc(PID *p);

#endif /* APP_PID_PID_H_ */
