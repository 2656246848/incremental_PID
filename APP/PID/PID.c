/*
 * PID.c
 *
 *  Created on: 2023年5月29日
 *      Author: gy
 */

#include "PID.h"

PID speed_pid;//结构体实例化要放到.c文件里

void PID_Init(PID *p)
{
    p->sv=1000.0f;    //100r/min
    p->pv=0.0f;
    p->system_scale = 9.60f;//速度0~960r/min
    p->Kp=P;
    p->Ki=I;
    p->Kd=D;
    p->EK=0.0f;
    p->Last_EK=0.0f;
    p->prev_EK=0.0f;
}

float Get_speed(PID *p,POSSPEED *p2)
{
    #if speed_method == speed_method_M
        if(p2->Speed_fr<750||p2->Speed_fr>-750)
            p->pv = p2->Speed_fr*10;
        else
            p->pv = 0;
    #endif
    #if speed_method == speed_method_T
        p->pv = p2->Speed_pr*10;
    #endif
    return p->pv;
}


void PID_Calc(PID *p)
{
    float add_OUT;
    if(p->EK<7500||p->EK>-7500)
    {
        p->EK = p->sv - p->pv;
    }
    add_OUT=p->Kp*(p->EK - p->Last_EK)+p->Ki*p->EK+p->Kd*(p->EK - 2*p->Last_EK + p->prev_EK);
    p->OUT += add_OUT/p->system_scale;

    p->prev_EK=p->Last_EK;
    p->Last_EK=p->EK;
}

