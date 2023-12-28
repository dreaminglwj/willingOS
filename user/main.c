#include "sysTick.h"
#include "willingOS.h"
#include "coreDef.h"
#include "task.h"

#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define LED1 PBout(5)  	
#define LED2 PEout(5)  

TaskHandle_t lightLed1Handler;
TaskHandle_t lightLed2Handler;
TaskHandle_t shutLedsHandler;

oid lightLed1( void *param ) {
    while (1)
    {
       LED1 = 1;
    }
    
}

void lightLed2( void *param ) {
    while (1)
    {
        LED2 = 1;
    }
    
}

void shutLeds( void *param ) {
    while(1) {
        LED1 = 0;
        LED2 = 0;
    }
}

int main() {
   OSErr err;
   initWilling(&err);

   // 创建任务
   ENTER_CRITICAL_SECTION();

   createTask( (TaskFunc_t) lightLed1,
               (const char *) "lightLed1",
               (uint32_t  ) 50,
               (void *) NULL,
               (UBase_t) 2,
               (TaskHandle_t *)lightLed1Handler);

    createTask( (TaskFunc_t) lightLed2,
            (const char *) "lightLed2",
            (uint32_t  ) 50,
            (void *) NULL,
            (UBase_t) 2,
            (TaskHandle_t *)lightLed2Handler);

    createTask( (TaskFunc_t) shutLeds,
            (const char *) "shutLeds",
            (uint32_t  ) 50,
            (void *) NULL,
            (UBase_t) 2,
            (TaskHandle_t *)shutLedsHandler);

   EXIT_CRITICAL_SECTION();

   willingStart(&err);

    return 0;
}

