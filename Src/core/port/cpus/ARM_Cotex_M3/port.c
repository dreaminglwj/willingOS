#include <stdint.h>
#include "coreDef.h"
#include "task.h"
#include "willing.h"
#include "willingOSConfig.h"
#include "stm32f10x.h"
#include "led.h"

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


uint32_t setInterruptMaskFromISR( void ) {
	__asm volatile( "mrs r0, PRIMASK  \n"
    "cpsid i  \n"
    "bx lr  \n");
	return 0;
}

void clearInterruptMaskFromISR( uint32_t mask ) {
    __asm volatile( "msr PRIMASK, r0  \n"
    "bx lr  \n");
}



/*  检查等待列表，将延时时间到的任务移到就绪表，并判断就绪表是否有超过1个任务，如果超过1个，则调度
*/
void sysTickHandler( void ) {
    /* SysTick 以最低中断优先级运行，因此当该中断执行时，必须取消屏蔽所有中断。因此，无需保存然后恢复中断屏蔽值，因为其值已知 - 因此使用稍快的 RaiseBASEPRI() 函数
代替 SET_INTERRUPT_MASK_FROM_ISR() */
    /* 屏蔽相关中断 */
    maximizeBasePriority();
    {
        tickCount++;
        if ( tickCount == 0 ) {
            tickCountSession = !tickCountSession;
        }
        
        processDelay();
        
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
    __asm volatile("cpsid i" ::: "memory");  /* 关中断 */

    criticalNesting++;

    if ( criticalNesting == 1 ) {
        willingAssert( (NVIC_INTERRUPUT_CTRL_REG & ICSR_NONE_VECTOR_ACTIVE_MASK ) == 0 );
    }
} 

void exitCriticalSection( void ) {
    willingAssert(criticalNesting);
    criticalNesting--;
    if ( criticalNesting == 0 ) {
       __asm volatile( "cpsie i" ::: "memory" );  /* 开中断 */
    }
}

/* 用于启动第一个任务 
 恢复currentTCB，currentTCB 指下一个会被调用的任务；
 获取topOfStack，即当前任务的栈顶地址；
在createTask的时候，需要模拟的 Cortex 的异常入栈顺序，做好数据初始化；

使用 LDMIA 指令，以 topOfStack 开始顺序出栈，先恢复R4~R11，同时 R0 递增；

将 R0 赋值给 PSP（因为出栈的时候，处理器会按照入栈的顺序去取 R4-R11、R14，而这些寄存器在我们创建任务的时候已经手动压栈）

将 BASEPRI 寄存器赋值为 0，即允许任何中断

最后执行 bx R14，告诉处理器 ISR 完成，需要返回，此刻处理器便会进行出栈操作，PC 被我们赋值成为了执行任务的函数的入口，任务正式跑起来；
*/
 void SVCHandler( void ) {
	__asm volatile(  /* 声明堆栈的对齐方式为8字节对齐 */
			"ldr	r3, currentTCBConstSVC \n"	/* 恢复currentPCB. */
	"ldr r1, [r3] \n"			/* 根据 currentTCB 获取到对应 taskTCB的地址. */
	"ldr r0, [r1] \n"			/* 获取第一个成员变量stack,即当前任务的栈顶地址. */
	"ldmia r0!, {r4-r11} \n"		/* 使用 LDMIA 指令，以 topOfStack 开始顺序出栈，先恢复 R4~R11，同时 R0 递增；*/
	"msr psp, r0 \n"				/* 将 R0 赋值给 PSP（因为出栈的时候，处理器会按照入栈的顺序去取 R4-R11、R14，而这些寄存器在我们创建任务的时候已经手动压栈） */
	"isb \n"
	"mov r0, #0 \n"
	"msr	basepri, r0  \n"
	"orr r14, #0xd \n"
	"bx r14 \n"
	"     \n"
	".align 4  \n"
	"currentTCBConstSVC: .word currentTCB  \n"
	);
}

/*
取 MSP 的初始值：先根据向量表的位置寄存器 VTOR (0xE000ED08) 来获取向量表存储的地址；
 再根据向量表存储的地址，取出第一个元素__initial_sp，写入 MSP；
 Cortex-M3 处理器，上电默认进入线程的特权模式，因此使用 MSP 作为堆栈指针;
 程序从上电开始运行到这里，经过一系列的函数调用，MSP 已经不是最开始初始化的位置；
所以通过 MSR 重新初始化 MSP，丢弃主堆栈中的数据； 当然，这是一条不归路，代码跑到这里，不会再返回之前的调用路径。
最后调用 svc 并传入系统调用号为 0 启动 SVC 中断

*/

void startFirstTask( void ) {
	/* Use the NVIC offset register to locate the stack. */
	__asm volatile(
			 "ldr r0, =0xE000ED08 \n"  // 0xE000ED08 地址处为VTOR（向量偏移量）寄存器，存储向量表起始地址
	"ldr r0, [r0] \n" // 启动文件中，最初地址存在的__initial_sp
	"ldr r0, [r0] \n" // 根据向量表实际存储地址，取出向量表中的第一项，向量表第一项存储主堆栈指针MSP的初始值

	/* Set the msp back to the start of the stack. */
	"msr msp, r0 \n" // 将__initial_sp的初始值写入MSP中
	/* Globally enable interrupts. */
	"cpsie i \n"
	"cpsie f \n"
	"dsb \n"
	"isb \n"
	/* Call SVC to start the first task. */
	"svc 0 \n"
	"nop \n"
			);
}

 void pendSVHandler( void ) {
	
//	extern criticalNesting;
//    extern currentTCB;
//    extern taskSwitchContext;
//
	 
    __asm volatile(
    "mrs r0, psp \n" /* 将当前任务的堆栈指针（PSP）加载到寄存器r0中 */
    "isb \n" /* 指令同步屏障 */

    "ldr r3, currentTCBConstPendSV \n" /* 将当前tcb的地址存储到r3 */
    "ldr r2, [r3] \n" /* 以r3中的内容为地址，将该地址中的数据传到R2中，取currentTCB地址 */

    "stmdb r0!, {r4-r11} \n" /* 保存剩下的寄存器值，R4~R11这些寄存器系统不会主动入栈，需要手动推到当前任务的堆栈 bug202401090001：这里保存寄存器的时候导致内存溢出 */
    "str r0, [r2] \n" /* 保存当前任务的栈顶指针：将r0寄存器的值（PSP）保存到currentTCB指向的任务控制块中，即将PSP的值保存为任务的堆栈指针，因为tcb中stacktop在最上面 */

   // 保护现场，调用函数更新下一个准备运行的新任务
    "stmdb sp!, {r3, r14} \n"
    // 设置优先级：进入临界区
    "mov r0, %0 \n"  //#MAX_SYS_INTERRUPT_PRIORITY \n"
    "msr basepri, r0 \n"
    "dsb \n"
    "isb \n"
    "bl taskSwitchContext \n"
    "mov r0, #0 \n"
    "msr basepri, r0 \n"
    "ldmia sp!, {r3, r14}  \n"// 退出临界区

    "ldr r1, [r3] \n" // 取出新的currentTCB值保存到r1
    "ldr r0, [r1] \n" // 取出新任务的栈顶指针
    "ldmia r0!, {r4-r11} \n" // 恢复手动保存的寄存器
    "msr psp, r0 \n" // 使psp指向新任务的栈顶
    "isb \n"
    "bx r14 \n" // 返回，硬件执行现场恢复，开始执行任务
    "      \n"
    ".align 4 \n"
    "currentTCBConstPendSV: .word currentTCB \n"
    ::"i"(MAX_SYS_INTERRUPT_PRIORITY)
	);
}

/* 用于测试pendsv是否被调用 */
void pendSVHandler1( void ) {

	extern void sharling();
	sharling();
	
	 //extern sharling;
	
   //bl sharling
	 //mov pc,lr
}
