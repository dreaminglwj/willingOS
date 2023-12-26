#include <stdlib.h>
#include "coreDef.h"
#include "task.h"
#include "willing.h"

#define INITIAL_XPSR			( 0x01000000 )
#define START_ADDRESS_MASK		( ( Stack_t ) 0xfffffffeUL )

Stack_t *initStack( Stack_t *topOfStack, TaskFunc_t taskFunc, void *params )
{

	topOfStack--; /* Offset added to account for the way the MCU uses the stack on entry/exit of interrupts. */
	*topOfStack = INITIAL_XPSR;	/* xPSR */
	topOfStack--;
	*topOfStack = ( ( Stack_t ) taskFunc ) & START_ADDRESS_MASK;	/* PC */
	topOfStack--;
	*topOfStack = ( Stack_t ) taskExitError;	/* LR */

	topOfStack -= 5;	/* R12, R3, R2 and R1. */
	*topOfStack = ( Stack_t ) params;	/* R0 */
	topOfStack -= 8;	/* R11, R10, R9, R8, R7, R6, R5 and R4. */

	return topOfStack;
}


__asm uint32_t setInterruptMaskFromISR( void ) {
    mrs r0, PRIMASK
    cpsid i
    bx lr
}

__asm void clearInterruptMaskFromISR( uint32_t mask ) {
    msr PRIMASK, r0
    bx lr
}

static void taskExitError( void )
{
	// configASSERT( uxCriticalNesting == ~0UL );
	// portDISABLE_INTERRUPTS();
	for( ;; );
}

/*  检查等待列表，将延时时间到的任务移到就绪表，并判断就绪表是否有超过1个任务，如果超过1个，则调度
*/
void sysTickHandler( void ) {
    /* SysTick 以最低中断优先级运行，因此当该中断执行时，必须取消屏蔽所有中断。因此，无需保存然后恢复中断屏蔽值，因为其值已知 - 因此使用稍快的 RaiseBASEPRI() 函数
代替 SET_INTERRUPT_MASK_FROM_ISR() */
    /* 屏蔽相关中断 */
    maximizeBasePriority();
    {
        /* 调用系统时钟节拍服务 */
        if ( sysTickService() == wTRUE ) {
            /* 
                如果需要进行调度，则进行任务切换
                出发pendsv中断，在pendsv中断中进行切换
             */
            NVIC_INTERRUPUT_CTRL_REG = NVIC_PENDSV_SET_BIT;
        }
    }
    /* 开启相关中断 */
   clearBasePriorityFromISR();
}

/* 
    sysTick中断函数
    todo: 考虑将这个函数换个位置
    todo: 将这个函数添加到中断向量表中，以便能够在中断时被正确调用
 */
void SysTick_Handler( void ) {
    if ( getSchedulerState() != SCHEDULER_STATE_WAITING ) {
        sysTickHandler();
    }
}

Base_t startScheduler( void ) {
    Base_t rlt = 0;

    return rlt;
}

void stopScheduler( void ) {

}

void enterCriticalSection( void ) {

} 

void exitCriticalSection( void ) {

}

/* 用于启动第一个任务 */
__asm void SVCHandler( void ) {

}

__asm startFirstTask( void ) {
    
}

__asm void pendSVHandler( void ) {

}