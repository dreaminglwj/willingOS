#include "sysTick.h"
#include "willingOS.h"
#include "coreDef.h"
#include "task.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)
#define GPIOF_BASE            (APB2PERIPH_BASE + 0x1C00)
#define GPIOG_BASE            (APB2PERIPH_BASE + 0x2000)

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

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

void lightLed1( void *param ) {
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
			
        LED1 = 1;
        LED2 = 1;
    }
}


//#include "system.h"

/*  LEDÊ±ÖÓ¶Ë¿Ú¡¢Òý½Å¶¨Òå */
#define LED1_PORT 			GPIOB   
#define LED1_PIN 			GPIO_Pin_5
#define LED1_PORT_RCC		RCC_APB2Periph_GPIOB

#define LED2_PORT 			GPIOE   
#define LED2_PIN 			GPIO_Pin_5
#define LED2_PORT_RCC		RCC_APB2Periph_GPIOE


//#define LED1 PBout(5)  	
//#define LED2 PEout(5)  	


//void LED_Init(void);


//#endif

void delay(u32 i)
{
	while(i--);
}

void initLed( void ) {
	GPIO_InitTypeDef GPIO_InitStructure;//¶¨Òå½á¹¹Ìå±äÁ¿
	
	RCC_APB2PeriphClockCmd(LED1_PORT_RCC|LED2_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED1_PIN;  //Ñ¡ÔñÄãÒªÉèÖÃµÄIO¿Ú
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //ÉèÖÃÍÆÍìÊä³öÄ£Ê½
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //ÉèÖÃ´«ÊäËÙÂÊ
	GPIO_Init(LED1_PORT,&GPIO_InitStructure); 	   /* ³õÊ¼»¯GPIO */
	GPIO_SetBits(LED1_PORT,LED1_PIN);   //½«LED¶Ë¿ÚÀ­¸ß£¬Ï¨ÃðËùÓÐLED
	
	GPIO_InitStructure.GPIO_Pin=LED2_PIN;  //Ñ¡ÔñÄãÒªÉèÖÃµÄIO¿Ú
	GPIO_Init(LED2_PORT,&GPIO_InitStructure); 	   /* ³õÊ¼»¯GPIO */
	GPIO_SetBits(LED2_PORT,LED2_PIN);   //½«LED¶Ë¿ÚÀ­¸ß£¬Ï¨ÃðËùÓÐLED
}

int main() {

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
	
	 	LED1 = 0;
 	  LED2 = 0;


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

