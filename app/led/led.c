#include "led.h"
#include "systick.h"
#include "willingOS.h"
#include "coreDef.h"
#include "task.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

uint16_t led1Stat = 0;
uint16_t led2Stat = 1;

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




void delay(u32 i)
{
	while(i--);
}

void initLed( void ) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED1_PORT_RCC|LED2_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED1_PIN;  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  
	GPIO_Init(LED1_PORT,&GPIO_InitStructure); 	
	GPIO_SetBits(LED1_PORT,LED1_PIN);   
	
	GPIO_InitStructure.GPIO_Pin=LED2_PIN;  
	GPIO_Init(LED2_PORT,&GPIO_InitStructure); 	   
	GPIO_SetBits(LED2_PORT,LED2_PIN);   
}


void testLed( void ) {
    while (1)
    {
        LED1 = 0;
        LED2 = 1;
        delay_ms(2000);
        LED1 = 1;
        LED2 = 0;
        delay_ms(2000);
    }
    
}
