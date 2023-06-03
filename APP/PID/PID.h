/*
 * PID.h
 *
 *  Created on: 2023��5��29��
 *      Author: gy
 */

#ifndef PID_H_
#define PID_H_

#include "eqep.h"
#include "speed.h"
extern POSSPEED qep_posspeed;//��ȡ�ٶ���Ϣ


/*PID������غ�*/
#define     P               1.0f        //2.5
#define     I               0.0f        //0
#define     D               0.0f        //0
#define     sampling_T      12000      //��������20ms��12000us��

/*PID�ṹ��*/
typedef struct
{
    float sv;//�趨ֵ
    float pv;//ʵ��ֵ
    float Kp;
    float Kd;
    float Ki;
    float EK;//����ƫ��
    float Last_EK;//�ϴ�ƫ��
    float prev_EK;//�ϴ�ƫ��
    float OUT;
    float system_scale;

}PID;



void PID_Init();
float Get_speed(PID *p,POSSPEED *p2);
//float Get_Average_Speed(PID *p, POSSPEED *p2);
void PID_Calc(PID *p);

#endif /* APP_PID_PID_H_ */
