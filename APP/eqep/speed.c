/*
 * speed.h
 *
 *  Created on: 2023��4��16��
 *      Author: gy
 */


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"     // Device Headerfile and Examples Include File
#include "speed.h"   // Example specific Include file

POSSPEED qep_posspeed=POSSPEED_DEFAULTS;

void  POSSPEED_Init(void)
{

    #if (CPU_FRQ_150MHZ)
      EQep1Regs.QUPRD=1500000;          // Unit Timer for 100Hz at 150 MHz SYSCLKOUT��λʱ��������������ڣ�����ֵ��0.01s        ���˼Ĵ������������������ڵ�λʱ���¼��ĵ�λ��ʱ�������ڼ�������Щ�¼��������Լ������eQEPλ����Ϣ��
    #endif
    #if (CPU_FRQ_100MHZ)
      EQep1Regs.QUPRD=1000000;          // Unit Timer for 100Hz at 100 MHz SYSCLKOUT
    #endif

    EQep1Regs.QDECCTL.bit.QSRC=00;      // QEP quadrature count mode ��������ģʽ

    EQep1Regs.QEPCTL.bit.FREE_SOFT=2;
    EQep1Regs.QEPCTL.bit.PCRM=10;       // PCRM=10 mode - QPOSCNT reset on first index event      QPOSCNT(λ�ü�����)��ʱ����Դ��QCLK
    EQep1Regs.QEPCTL.bit.UTE=1;         // Unit Timeout Enable
    EQep1Regs.QEPCTL.bit.QCLM=1;        // Latch on unit time out   ��λʱ���¼�����ʱ�������ݡ�λ�ü�����ֵ�������ʱ���Ͳ�������ֵ������QPOSLAT��QCTMRLAT��QCPRDLAT�Ĵ����У���֤���񵽵�ֵ����ͬһ��ʱ���׼�£�
    EQep1Regs.QPOSMAX=pulse_count*10;       //λ�ü����������ֵ�����Ը��ݵ��һȦ��������ȷ��.����10Ȧ����һ��
    EQep1Regs.QEPCTL.bit.QPEN=1;        // QEP enable

    EQep1Regs.QCAPCTL.bit.UPPS=1;       // 1/2 for unit position  2��QCLK����һ��UPEVENT����
    EQep1Regs.QCAPCTL.bit.CCPS=7;       // 1/128 for CAP clock
    EQep1Regs.QCAPCTL.bit.CEN=1;        // QEP Capture Enable
    //EQep1Regs.QEPCTL.bit.IEI=2;         //�����ź�������ʱ��ʼ������λ�ü�����
    //EQep1Regs.QEPCTL.bit.SWI=1;//�����λ eQEP ��ϵͳ������,����һ�����������������תλ�û����У׼
}

void POSSPEED_Calc(POSSPEED *p)
{

     unsigned int newp,oldp;//����M�������ٶȵ��м���
     //int temp1;//����T�������ٶȵ��м���

     p->DirectionQep = EQep1Regs.QEPSTS.bit.QDF;    //����

// Check an index occurrence
     if (EQep1Regs.QFLG.bit.IEL == 1)   //�����¼��ж�
     {
        p->index_sync_flag = 0x00F0;
        EQep1Regs.QCLR.bit.IEL=1;                   // Clear interrupt flag
     }


    if(EQep1Regs.QFLG.bit.UTO==1)                    // If unit timeout (one 100Hz period) T��֪��0.03s������X = QPOSLAT��k��-QPOSLAT��k-1��
    {

        newp=(unsigned int)EQep1Regs.QPOSLAT;
        oldp=p->oldpos;

        if (p->DirectionQep==0)                     // POSCNT is counting down��TmpΪ����
        {
            if (newp>oldp)
                p->Pos_increment = -EQep1Regs.QPOSMAX + newp - oldp;
            else
                p->Pos_increment = newp -oldp;
        }
        else if (p->DirectionQep==1)                // POSCNT is counting up
        {
            if (newp>=oldp)
                p->Pos_increment = newp - oldp;
            else
                p->Pos_increment = EQep1Regs.QPOSMAX - oldp + newp;                     // x2-x1 should be positive
        }
        p->Speed_fr = p->Pos_increment*p->BaseRpm;              //      r/min
        //_IQ15mpy(10000,p->BaseRpm_Q)���Ƚ�10000��p->BaseRpm_Qת��Ϊlong�ͣ�Ȼ������������ٳ���2^15 (long���ֵ 4294967295)
        p->SpeedQ_fr= _IQ15mpy(p->Pos_increment*100,p->BaseRpm_Q); // ��������,Q15*Q0������õ���ʵֵ��100������ֹС���㱻ʡ��,�����ԣ���p->Pos_increment*100��100��Ϊ10ʱ��p->SpeedQ_fr��ֵʱ���������䣬��������pid����
        // Update the electrical angle
        p->oldpos = newp;
        EQep1Regs.QCLR.bit.UTO=1;                   // Clear interrupt flag
    }

//**** Low-speed computation using QEP capture counter ****//
    //UPEVNT����ֵ�����浽QCPRD�Ĵ����ı�־������λλ���¼�
    //��λλ�¼�֮��������ܳ���65535��
    //��λλ���¼�֮���޷���仯
    if(EQep1Regs.QEPSTS.bit.UPEVNT==1)                 // Unit position event 32��QCLK��8����������������.    X��֪��UPPS���ƣ�����T�����ڲ������ڼĴ���QCPRDLAT
    {
        if(EQep1Regs.QEPSTS.bit.COEF==0)               // No Capture overflow û�����
            p->temp1=(unsigned long)EQep1Regs.QCPRDLAT;   // temp1 = t2-t1
        else                                           // Capture overflow, saturate the result
            p->temp1=0xFFFF;
        if(EQep1Regs.QEPSTS.bit.CDEF==1)//û�з���仯
            p->Speed_pr = p->SpeedScaler/p->temp1;    // p->Speed_pr = p->SpeedScaler/temp1  ���IQ24 ʵ�ֹ������ں������ֽ�p->SpeedScaler,temp1ת��Ϊlong�ͣ�Ȼ������õ�ΪIQ24
        else
            p->Speed_pr = p->Speed_pr;
        EQep1Regs.QEPSTS.all=0x88;                  // Clear Unit position event flag
                                                    // Clear overflow error flag
    }


}

