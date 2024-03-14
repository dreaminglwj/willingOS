/*
 * steer.c
 *
 *  Created on: Mar 13, 2024
 *      Author: Administrator
 */


#include "steer.h"
#include "stm32f10x.h"

void SteerMotorInit(uint16_t pwmPeriod) {
 // 使能Timer1和GPIOA
 RCC_APB2PeriphClockCmd( SteerMotorCtlTimerRCC, ENABLE );
 RCC_APB2PeriphClockCmd( SteerMotorCtlPortRCC, ENABLE );

 // 配置转向舵机pwm信号管脚
 GPIO_InitTypeDef GPIOA_InitStruct;
 GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;// 推挽输出模式
 GPIOA_InitStruct.GPIO_Pin = SteerMotorCtlPinAddr;
 GPIOA_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init( SteerMotorCtlPort, &GPIOA_InitStruct );

 TIM_InternalClockConfig(SteerMotorPWMTimer);

 TIM_TimeBaseInitTypeDef SteerMotorTimer_InitStruct;
 SteerMotorTimer_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // 不分频
 SterrMotorTimer_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;// 边缘对齐模式，计数器依据方向位（DIR）向上计数
 SteerMotorTimer_InitStruct.TIM_Period = pwmPeriod; // ARR，自动重装载寄存器 设置时钟周期，按照72分频，时钟为1M，1ms为1000个时钟周期
 SteerMotorTimer_InitStruct.TIM_Prescaler = PreScaler_72Div; // PSC 预分频器，系统时钟通过预分频
 SteerMotorTimer_InitStruct.TIM_RepetitionCounter = 0; // 倍数寄存器，比如周期为20ms，counter为0表示20ms溢出，为1表示20*2ms溢出
    TIM_TimeBaseInit(SteerMotorPWMTimer, &SteerMotorTimer_InitStruct );

    TIM_OCInitTypeDef SteerMotorOCInitStruct;
    TIM_OCStructInit( &SteerMotorOCInitStruct );
    SteerMotorOCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;  // PWM1:向上计数：CNT<CCR时，REF置有效电平，CNT>=CCR时，REF置无效电平
                                                          // PWM1:向下计数：CNT>CCR时，REF置无效电平，CNT<=CCR时，REF置有效电平
                                                          // PWM2:向上计数：CNT<CCR时，REF置无效电平，CNT>=CCR时，REF置有效电平
                                                          // PWM2:向下计数：CNT>CCR时，REF置有效电平，CNT<=CCR时，REF置无效电平
    SteerMotorOCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; // 输出极性为高电平有效
    SteerMotorOCInitStruct.TIM_OutputState = TIM_OutputState_Enable; // 使能输出状态
    SteerMotorOCInitStruct.TIM_Pulse = 0; // ccr  空比，通过修改TIM1->CCR1-4寄存器来实现，这里不统一设置
    TIM_OC1Init( SteerMotorPWMTimer, &SteerMoto
