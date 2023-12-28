#ifndef WILLING_OS_CONFIG_H
#define WILLING_OS_CONFIG_H

#define MAX_TASK_NAME_LEN (16) 
#define MAX_PRIORITY_NUM  (32)

/******************************************************************
            时钟相关定义                                                
******************************************************************/
#define SYS_TICK_RATE ((uint32_t) 1000)
#define SYS_TICK_CLOCK_RATE (SysClockFrequence)

/******************************************************************
            与中断有关的配置选项                                                 
******************************************************************/
#ifdef __NVIC_PRIORITY_BITS
    #define PRIORITY_BITS __NVIC_PRIORITY_BITS
#else
    #define PRIORITY_BITS 4
#endif

// 中断最低优先级，字面量，给人看的值，在实际使用时需要进行转换
#define LOWEST_INTERRUPT_PRIORITY_LITERAL 15

/* 系统可管理的最高中断优先级 */
#define MAX_INTERRUPT_PRIORITY_LITERAL 5

#define KERNEL_INTERRUPT_PRIORITY 		( LOWEST_INTERRUPT_PRIORITY_LITERAL << (8 - PRIORITY_BITS) )	/* 240 */

/* 实际使用的优先级 */
#define MAX_SYS_INTERRUPT_PRIORITY ( MAX_INTERRUPT_PRIORITY_LITERAL << ( __CORTEX_M3_INTERRUPT_PRIORITY_BITS - PRIORITY_BITS ) )

/****************************************************************
            绑定中断服务函数                        
****************************************************************/
#define pendSVHandler 	PendSV_Handler
#define SVCHandler 	SVC_Handler

#endif
