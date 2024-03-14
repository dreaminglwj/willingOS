/*
 * steer.h
 *
 *  Created on: Mar 13, 2024
 *      Author: lwj
 *      定义舵机型号，不同的型号有不同的参数，根据型号来获取参数，从而实现控制
 */

#ifndef APP_STEER_STEER_H_
#define APP_STEER_STEER_H_

#include "gpio.h"
#include "stm32f10x_rcc.h"

#define WillingCarV1_SteerLeft_MaxAngle -45
#define WillingCarV1_SteerRight_MaxAngle 45

#define Period_20ms_BaseOn72Div (20000 -1) // 基于72M时钟，不分频，产生20ms周期的pwm
#define PreScaler_72Div  (72-1) // 预分频，采用72分频，分频后的时钟为1M


#define SteerMotorCtlPinAddr GPIO_Pin_1;
#define SteerMotorCtlPin PAOut(1);


#define SteerMotorCtlPortRCC RCC_APB2Periph_GPIOA // 舵机控制端口复位时钟控制
#define SteerMotorPWMTimerRCC RCC_APB2Periph_TIM1 // 舵机控制timer复位时钟控制

#define SteerMotorCtlPort GPIOA // 舵机控制端口
#define SteerMotorPWMTimer TIM1 // 舵机pwmTimer

extern void SteerMotorInit(uint16_t pwmPeriod); // 初始化舵机控制timer和端口
extern void SetSteerMotorDutyRatio( uint16_t dutyRatio ); // 设置转向舵机pwm占空比，上限1000，超过1000按照1000计算，10代表1%
extern void TurnSteerMotor( int16_t angle ); // 舵机转向，正表示左转，负表示右转。提供给车子的转向函数用，车子的转向需要根据车身建模，车子转向角度不等于舵机的转向角度

#endif /* APP_STEER_STEER_H_ */
