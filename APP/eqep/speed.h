/*
 * speed.h
 *
 *  Created on: 2023年4月16日
 *      Author: gy
 */

#ifndef APP_EQEP_SPEED_H_
#define APP_EQEP_SPEED_H_

#include "IQmathLib.h"         // Include header for IQmath library

//电机物理信息:线数、减速比
#define lin_number      13
#define raduction_ratio     20
#define pulse_count     1040    //一圈QPOSCNT加1040
#define set_SpeedScaler   135216.34615384
#define speed_method        2
#define speed_method_T      1
#define speed_method_M      2

typedef struct {
                int DirectionQep;       // Output: Motor rotation direction (Q0)
                int QEP_cnt_idx;        // Variable: Encoder counter index (Q0)
                int pole_pairs;         // Parameter: Number of pole pairs (Q0)
                int index_sync_flag;    // Output: Index sync status (Q0)

                double SpeedScaler;     // Parameter :  Scaler converting 1/N cycles to a GLOBAL_Q speed (Q0) - independently with global Q
                float Speed_pr;           // Output :  speed in per-unit
                float BaseRpm;         // BaseRpm=60/(1040*0.03)
                _iq BaseRpm_Q;         //BaseRpm_Q=60*2^15=189046  (Q15)
                int32 SpeedRpm_pr;      // Output : speed in r.p.m. (Q0) - independently with global Q

                int temp1;
                int Pos_increment;
                int  oldpos;            // Input: Electrical angle (pu)
                float Speed_fr;           // Output :  speed in per-unit
                _iq SpeedQ_fr;      // Output : Speed in rpm  (Q0) - independently with global Q
                float average;
                void (*init)();         // Pointer to the init funcion
                void (*calc)();         // Pointer to the calc funtion
                }  POSSPEED;

/*-----------------------------------------------------------------------------
Define a POSSPEED_handle
-----------------------------------------------------------------------------*/
typedef POSSPEED *POSSPEED_handle;

/*-----------------------------------------------------------------------------
Default initializer for the POSSPEED Object.
-----------------------------------------------------------------------------*/
/*这里“\”用于连接多行代码*/
#if (CPU_FRQ_150MHZ)
  #define POSSPEED_DEFAULTS {0x0,0x0,2,0x0,\
        set_SpeedScaler,0,5.76923076923,189046,0,\
        0,0,0,0,0,0,\
        (void (*)(long))POSSPEED_Init,\
        (void (*)(long))POSSPEED_Calc }
#endif
#if (CPU_FRQ_100MHZ)
  #define POSSPEED_DEFAULTS {0x0,0x0,2,0x0,\
        2*set_SpeedScaler/3,0,5.76923076923,189046,0,\
        0,0,0,0,0,0,\
        (void (*)(long))POSSPEED_Init,\
        (void (*)(long))POSSPEED_Calc }
#endif

void POSSPEED_Init(void);
void POSSPEED_Calc(POSSPEED_handle);

#endif /* APP_EQEP_SPEED_H_ */
