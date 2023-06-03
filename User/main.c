#include "main.h"
#include "INIT.h"

Uint16 va = 0;

void DELAY_MS(int b)
{
   int a;
   while(b)
   {

          for(a=0;a<1000;a++)
          {
              DELAY_US(1);
          }
          b--;
   }

}

void main()
{
    #if RUN_TYPE==FLASH_RUN
        MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
        InitFlash();
    #endif
    SYSTERM_INIT();

    if(1)
    {
        OLED_DisShowCHinese(56,5,2);
        OLED_DisShowCHinese(74,5,3);
        OLED_DisShowCHinese(92,5,4);
        OLED_ShowString(0,0,"freq:150kHz",16);
        OLED_ShowString(90,0,"IO0",16);
        OLED_ShowString(0,2,"duty:",16);
        OLED_ShowNum(70,2,va/150,2,2,16);
        OLED_ShowString(0,4,"K1/K2",16);
        OLED_ShowString(0,6,"10/1",16);
    }

    #if RUN_TYPE==SRAM_RUN
        // 正常
        printf("Hello World\n");
//        char buffer[] ="3.33";
//        printf("%s\n",buffer);
//        printf("%d\n",33);
//

        //printf("%d\n",qep_posspeed.DirectionQep);
        //这样上位机就可以将两个数据分开制作波形
        //printf("%.2d,%.2d\n",33,qep_posspeed.DirectionQep);//这里打印了多少个就是多少个通道，上位机会识别

    #endif

    while (1)
    {
        printf("%d,%d,%d,%d\n",(int)(speed_pid.pv),(int)speed_pid.sv,(int)speed_pid.OUT,va);

        EPwm1B_SetCompare(va);//va范围0~15000
        EPwm1A_SetCompare(va);
    }

}
