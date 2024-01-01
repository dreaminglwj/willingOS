#include <stdint.h>
#include "coreDef.h"
#include "task.h"
#include "willing.h"
#include "willingOSConfig.h"
#include "stm32f10x.h"

#ifdef SYSCLK_FREQUENCE_72MHz
    uint32_t SysClockFrequence = SYSCLK_FREQUENCE_72MHz;
#endif

#define INITIAL_XPSR			( 0x01000000 )
#define START_ADDRESS_MASK		( ( Stack_t ) 0xfffffffeUL )
/* ICSR寄存器中除了VECTACTIVE位之外的其他所有位的掩码 */
#define ICSR_NONE_VECTOR_ACTIVE_MASK ( 0xFFUL )

#define NVIC_PENDSV_PRI					( ( ( uint32_t ) KERNEL_INTERRUPT_PRIORITY ) << 16UL )
#define NVIC_SYSTICK_PRI				( ( ( uint32_t ) KERNEL_INTERRUPT_PRIORITY ) << 24UL )

#define NVIC_SYS_PRI2_REG               ( * ( ( volatile uint32_t * ) 0xE000ED20 ) )

#define NVIC_SYSTICK_CLK_BIT	   ( 1UL << 2UL )
#define NVIC_SYSTICK_INT_BIT       ( 1UL << 1UL )
#define NVIC_SYSTICK_ENABLE_BIT	   ( 1UL << 0UL )

static void taskExitError( void )
{
	// configASSERT( uxCriticalNesting == ~0UL );
	// portDISABLE_INTERRUPTS();
	for( ;; );
}

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
// void SysTick_Handler( void ) {
//     if ( getSchedulerState() != SCHEDULER_STATE_WAITING ) {
//         sysTickHandler();
//     }
// }

/*
    确定一个最高ISR优先级，在这个ISR或者更低优先级的ISR中可以安全的调用以FromISR结尾的API函数
*/
void ensureHighestISRPriority( void ) {
//     volatile uint32_t priorityBackup;
//     volatile uint8_t * const firstUserPriorityReg = ( uint8_t * ) ( NVIC_IP_REGISTERS_OFFSET_16 + FIRST_USER_INTERRUPT_NUMBER );
//     volatile uint8_t maxPriority;

//     /* 保存中断优先级值，以便覆写PRI_0寄存器值 */
//     priorityBackup = *firstUserPriorityReg;

//     /* 
//         确定有效的优先级位数
//         首先向所有位写1，然后再读出来，由于无效的优先级位读出来为0，所以最后计算以下值为1的位数就知道有多少优先级位数了
//      */
//     *firstUserPriorityReg = MAX_8_BIT_VALUE;
//     maxPriority = *firstUserPriorityReg;

//     /*
//         冗余代码，用于防止用户错误设置可屏蔽中断优先级值
//     */
//    max

}

void setupTimerInterrupt( void ) {
    SysTick->LOAD = ( SYS_TICK_CLOCK_RATE / SYS_TICK_RATE ) - 1UL;
    SysTick->CTRL = ( NVIC_SYSTICK_CLK_BIT | NVIC_SYSTICK_INT_BIT | NVIC_SYSTICK_ENABLE_BIT );
}


static void startFirstTask( void );

Base_t startWillingScheduler( void ) {
    Base_t rlt = 0;

    /* 将PendSV和systick中断优先级设置为最低 */
    NVIC_SYS_PRI2_REG |= NVIC_PENDSV_PRI;
    NVIC_SYS_PRI2_REG |= NVIC_SYSTICK_PRI;

    setupTimerInterrupt();

    criticalNesting = 0;

    startFirstTask();

    return rlt;
}

void stopWillingScheduler( void ) {
    willingAssert(criticalNesting==1000UL);
}

void enterCriticalSection( void ) {
    __asm("cpsid i");  /* 关中断 */

    criticalNesting++;

    if ( criticalNesting == 1 ) {
        willingAssert( (NVIC_INTERRUPUT_CTRL_REG & ICSR_NONE_VECTOR_ACTIVE_MASK ) == 0 );
    }
} 

void exitCriticalSection( void ) {
    willingAssert(criticalNesting);
    criticalNesting--;
    if ( criticalNesting == 0 ) {
       __asm( "cpsie i" );  /* 开中断 */
    }
}

/* 用于启动第一个任务 */
__asm void SVCHandler( void ) {
	PRESERVE8

	ldr	r3, =currentTCB	/* Restore the context. */
	ldr r1, [r3]			/* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
	ldr r0, [r1]			/* The first item in pxCurrentTCB is the task top of stack. */
	ldmia r0!, {r4-r11}		/* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
	msr psp, r0				/* Restore the task stack pointer. */
	isb
	mov r0, #0
	msr	basepri, r0
	orr r14, #0xd
	bx r14
	nop
}


__asm void startFirstTask( void ) {
	PRESERVE8

	/* Use the NVIC offset register to locate the stack. */
	ldr r0, =0xE000ED08
	ldr r0, [r0]
	ldr r0, [r0]

	/* Set the msp back to the start of the stack. */
	msr msp, r0
	/* Globally enable interrupts. */
	cpsie i
	cpsie f
	dsb
	isb
	/* Call SVC to start the first task. */
	svc 0
	nop
	nop
}

__asm void pendSVHandler( void ) {
    extern criticalNesting;
    extern currentTCB;
    extern taskSwitchContext;

    PRESERVE8

    mrs r0, psp /* 将当前任务的堆栈指针（PSP）加载到寄存器r0中 */
    isb /* 指令同步屏障 */

    ldr r3, =currentTCB /* 将当前tcb的地址存储到r3 */
    ldr r2, [r3] /* 以r3中的内容为地址，将该地址中的数据传到R2中 */

    stmdb r0!, {r4-r11} /* 保存剩下的寄存器值 */
    str r0, [r2] /* 将r0寄存器的值（PSP）保存到currentTCB指向的任务控制块中，即将PSP的值保存为任务的堆栈指针，因为tcb中stack在最上面 */

    stmdb sp!, {r3, r14}
    mov r0, #MAX_SYS_INTERRUPT_PRIORITY  
    msr basepri, r0
    dsb
    isb
    bl taskSwitchContext
    mov r0, #0
    msr basepri, r0
    ldmia sp!, {r3, r14}

    ldr r1, [r3]
    ldr r0, [r1]
    ldmia r0!, {r4-r11}
    msr psp, r0
    isb
    bx r14
    nop
}
