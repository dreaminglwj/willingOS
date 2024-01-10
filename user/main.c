#include "sysTick.h"
#include "willingOS.h"
#include "coreDef.h"
#include "task.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "led.h"
#include <stdlib.h>
#include "timer.h"

TaskHandle_t lightLed1Handler;
TaskHandle_t lightLed2Handler;
TaskHandle_t shutLedsHandler;

void testMem(void)
{
   void *memAddr = NULL;
   uint32_t count = 0;

   for (count = 0; count < 0x1000; count++)
   {
      memAddr = malloc(count);
      if (memAddr == NULL)
      {
         int i = 0;
         i++;
      }
      else
      {
         free(memAddr);
      }
   }
}





void timerTaskLed1(void *param)
{
   led1Stat = !led1Stat;
   LED1 = led1Stat;
}






int main()
{

   // 	initLed();

   // 	LED1 = 0;
   // 	LED2 = 0;

   //   while(1)
   // 	{
   // 		LED1=!LED1;
   // 		LED2=!LED2;
   // 		delay(6000000);
   // 	}

   OSErr err;
   initWilling(&err);
   initLed();

   LED1 = led1Stat;
   LED2 = led2Stat;

   // testMem();

   // testLed();
   /*
    while(1) {
       ;
    }
*/

   // 创建任务
   ENTER_CRITICAL_SECTION();







/*   createTimer_ms(
       1,
       500, 
       TIMER_MOD_REPEAT,
       timerTaskLed1,
       NULL);
*/






// todo: 定时器时间不对，可能也跟timerTask函数有关



/* bug:202401102317,timer调通了，task挂了*/
      createTask( (TaskFunc_t) lightLed1,
                  (const char *) "lightLed1",
                  (uint32_t  ) 24,
                  (void *) NULL,
                  (UBase_t)SUGGESTED_PRIORITY,
                  (TaskHandle_t *)lightLed1Handler);



/*       createTask( (TaskFunc_t) lightLed2,
               (const char *) "lightLed2",
               (uint32_t  ) 24,
               (void *) NULL,
               (UBase_t) SUGGESTED_PRIORITY,
               (TaskHandle_t *)lightLed2Handler);*/

   //     createTask( (TaskFunc_t) shutLeds,
   //             (const char *) "shutLeds",
   //             (uint32_t  ) 50,
   //             (void *) NULL,
   //             (UBase_t) SUGGESTED_PRIORITY,
   //             (TaskHandle_t *)shutLedsHandler);

   EXIT_CRITICAL_SECTION();

   willingStart(&err);

   return 0;
}
