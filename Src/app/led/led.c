#include "led.h"
#include "systick.h"
#include "willingOS.h"
#include "coreDef.h"
#include "task.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "task.h"

uint16_t led1Stat = 0;
uint16_t led2Stat = 1;

uint32_t countMax = 360000;

void sharling(void) {
	led1Stat = !led1Stat;
	led2Stat = !led2Stat;
	
	LED1 = led1Stat;
	LED2 = led2Stat;
	led1Stat = led1Stat;
}


void lightLed1( void *param ) {
    while (1)
    {
        led1Stat = !led1Stat;
        LED1 = led1Stat;
        willingSleep_ms(500);
    }
}


void lightLed2( void *param ) {
    // uint32_t count = 0;
    while (1)
    {
    //     count++;
    //     if (count >= countMax) {
    //         count = 0;
    //         led2Stat = !led2Stat;
    //     }
    //    LED2 = led2Stat;
        led2Stat = !led2Stat;
        LED2 = led2Stat;
        willingSleep_ms(500);
        //led2Stat = !led2Stat;
        //LED2 = led2Stat;
    }
}

void shutLeds( void *param ) {
    while(1) {
			;
        // LED1 = 1;
        // LED2 = 1;
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
        willingSleep_ms(200);
        LED1 = 1;
        LED2 = 0;
        willingSleep_ms(200);
    }
    
}
