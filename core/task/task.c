#include <stdint.h>
#include <string.h>

#include "coreDef.h"
#include "task.h"
#include "willing.h"
#include "WillingOSConfig.h"
#include "memory.h"
#include "protable.h"


typedef struct taskControlBlock {
    volatile Stack_t  *stackTop; /* 栈顶指针，指向最后一个入栈的元素 */
    Stack_t *stack; /* 栈底指针，指向栈的起始位置 */
    ULong_t priority;

    /* delay相关 */
    uint32_t delayExpireAt; /* 延时结束时间，为n个tickCount，0表示没有延时 */
    uint8_t   tickCountSession; /* tickCount会话 */

    char taskName[ MAX_TASK_NAME_LEN ]; /* 任务名称 */
} TCB;


/* 重新定义一个tcb变量，供编程使用 */
typedef TCB TCB_t;

static volatile Base_t schedulerRunning = wFALSE;
static volatile Base_t schedulerSuspended = wFALSE;
static volatile uint32_t tickCount      = 0U;
static volatile uint8_t  tickCountSession = 0U;


TCB_t * taskArrayForTest[MAX_TASK_NUM];
uint16_t taskNum = 0;
TCB_t * volatile currentTCB = NULL;
uint16_t currentTaskIndex =0;



static void initTask( TaskFunc_t taskFunc,
                            const char * const taskName,
                            const uint32_t stackSize,
                            void * const params,
                            UBase_t priority,
														TCB_t * tcb,
														TaskHandle_t * const taskHandler);


/* 用于测试任务调度，暂时未加临界区，当三个任务都加入到数组后开启调度器 */
static void addTaskToReadyArray( TCB_t * tcb );



Long_t createTask( TaskFunc_t taskFunc, /* 任务函数 */ 
                    const char* const taskName,  /* 任务名称 */ 
                    const uint32_t stackSize, /* 栈大小 */ 
                    void * const params, /* 传递给taskFunc的参数 */ 
                    ULong_t priority, /* 优先级 */ 
                    TaskHandle_t * const taskHandler /* 任务句柄 */ ) {
    TCB_t * tcb = NULL;
    Long_t rlt = -1;


    /* 堆栈向上增长（正向生长，51单片机）：从低地址（0x00）向高地址(0xFF)增长
    堆栈向下增长（逆向生长，80x86和cotex-M3）：从高地址(0xFF)向低地址（0x00）增长 */
    /* 如果栈向下生长，先给stack分配空间再给tcb分配空间，防止栈增长覆盖tcb内存；
    如果栈向上生长，先给tcb分配空间，再给stack分配空间 */
    #if(STACK_GROWTH == STACK_GROWTH_DOWN) 
		{
        Stack_t *stack;
        
        stack = ( Stack_t *  ) willingMalloc( ( (size_t) stackSize ) * sizeof(Stack_t) );

        if ( stack != NULL ) {
            tcb = ( TCB_t * ) willingMalloc( sizeof( TCB_t ) );

            if ( tcb != NULL ) {
                tcb->stack = stack;
            } else {
                willingFree( stack );
            }
        } 
    } 
    #else 
		{ /* STACK_GROWTH */

    }
    #endif /* STACK_GROWTH */

    if ( tcb != NULL  ) {
        initTask(taskFunc, taskName, stackSize, params, priority, tcb, taskHandler);
        addTaskToReadyArray(tcb);
        rlt = wPASS;
    } else {
        rlt = errMEMORY_ALLOCATE_FAIL;
    }
    return rlt;
}


static void initTask( TaskFunc_t taskFunc,
                            const char * const taskName,
                            const uint32_t stackSize,
                            void * const params,
                            UBase_t priority,
							TCB_t * tcb,
							TaskHandle_t * const taskHandler) {
    Stack_t * topOfStack = NULL;
    UBase_t i = 0;

    #if( STACK_GROWTH == STACK_GROWTH_DOWN ) 
		{
        topOfStack = tcb->stack + ( stackSize - ( uint32_t ) 1 );
        topOfStack = ( Stack_t * ) ( ( ( willingPOINTER_SIZE_TYPE ) topOfStack ) & ( ~( ( willingPOINTER_SIZE_TYPE ) BYTE_ALIGNMENT_MASK ) ) );

        // 检查溢出
    }
    #else 
		{  /* STACK_GROWTH */

    }
    #endif /* STACK_GROWTH */

    /* 拷贝taskName */
    for ( i = ( UBase_t ) 0; i < (UBase_t) MAX_TASK_NAME_LEN; i++ ) {
        tcb->taskName[ i ] = taskName[ i ];
        if ( taskName[i] == 0x00 ) {
            break;
        } else {
            //
        }
    }

    tcb->taskName[ MAX_TASK_NAME_LEN - 1 ] = 0x00;

    /* 赋值priority */
    if ( priority >= ( UBase_t ) MAX_PRIORITY_NUM ) {
        tcb->priority = ( UBase_t ) MAX_PRIORITY_NUM - ( UBase_t )1U;
    } else {
        tcb->priority = priority;
    }

    tcb->delayExpireAt =  DELAY_VALUE_NO_DELAY;
    tcb->tickCountSession = tickCountSession;

    tcb->stackTop = initStack( topOfStack, taskFunc, params );

    if ( ( void * ) taskHandler != NULL ) {
        *taskHandler = ( TaskHandle_t ) tcb;
    } else {
        /* 错误处理 */
    }

}


/* 用于测试任务调度，暂时未加临界区，当三个任务都加入到数组后开启调度器 */
static void addTaskToReadyArray( TCB_t * tcb ) {
	uint16_t* taskNumAddr = &taskNum;
	TCB_t ** taskArrayForTestAddr = taskArrayForTest;
	TCB_t ** taskArrayForTestAddr2 = taskArrayForTest + 2;
    if ( taskNum > 2 ) {
        return;
    }

    taskArrayForTest[taskNum] = tcb;
		taskNum++;
		taskNum = taskNum;
        // todo：检查范围，否则存在越界风险
    currentTCB = taskArrayForTest[currentTaskIndex];
}

/*  
    统时钟服务函数，
    调度算法函数在这个函数中调用
*/
Base_t sysTickService( void ) {
    Base_t needSwitchCtx = wFALSE;
    // 暂时默认需要切换
    needSwitchCtx = wTRUE;

    return needSwitchCtx;
}



Base_t getSchedulerState( void ) {
    Base_t rlt = SCHEDULER_STATE_RUNNING;

    /* todo: 完成具体实现 */
	if ( schedulerRunning == wFALSE ) {
		rlt = SCHEDULER_STATE_WAITING;
	} else {
		if ( schedulerSuspended == wFALSE ) {
			rlt = SCHEDULER_STATE_RUNNING;
		} else {
			rlt = SCHEDULER_STATE_SUSPENDED;
		}
	}
    
    return rlt;
}

void taskSwitchContext( void ) {
    // 将curTcb指向下一个就绪的任务控制块
    willingAssert( taskNum > 0 );

    // todo: 以下操作会导致第一次启动的时候任务从第二个任务启动
    currentTaskIndex++;
    if ( currentTaskIndex >= taskNum ) { // 环状
        currentTaskIndex = 0;
    }

    currentTCB = taskArrayForTest[currentTaskIndex];
}

void idleTask(void) {
	while(1) {
		int i = 0;
		i=1;
		;
	}
}

void OSStart(void) {
    DISABLE_INTERRUPTS();
    schedulerRunning = wTRUE;
    tickCount = 0U;

    if ( startWillingScheduler() != wFALSE ) {

    } else {

    }
		
		idleTask();
}

void OSStop(void) {

}



