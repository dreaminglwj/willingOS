#include <stdlib.h>
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

    char taskName[ MAX_TASK_NAME_LEN ]; /* 任务名称 */
} TCB;


/* 重新定义一个tcb变量，供编程使用 */
typedef TCB TCB_t;


TCB_t * taskArrayForTest[3];
uint16_t taskNum = 0;



static void initTask( TaskFunc_t taskFunc,
                            const char * const taskName,
                            const uint32_t stackSize,
                            void * const params,
                            UBase_t priority);


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
    #if(STACK_GROWTH == STACK_GROWTH_DOWN) {
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
    #else { /* STACK_GROWTH */

    }
    #endif /* STACK_GROWTH */

    if ( tcb != NULL  ) {
        initTask(taskFunc, taskName, stackSize, params, priority, taskHandler);
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
                            TaskHandle_t * const taskHandler,
                            TCB_t * tcb ) {
    Stack_t * topOfStack = NULL;
    UBase_t i = 0;

    #if( STACK_GROWTH == STACK_GROWTH_DOWN ) {
        topOfStack = tcb->stack + ( stackSize - ( uint32_t ) 1 );
        topOfStack = ( Stack_t * ) ( ( ( willingPOINTER_SIZE_TYPE ) topOfStack ) & ( ~( ( willingPOINTER_SIZE_TYPE ) BYTE_ALIGNMENT_MASK ) ) );

        // 检查溢出
    }
    #else {  /* STACK_GROWTH */

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

    tcb->topOfStack = initStack( topOfStack, taskFunc, params );

    if ( ( void * ) taskHandler != NULL ) {
        *taskHandler = ( TaskHandle_t ) tcb;
    } else {
        /* 错误处理 */
    }

}


/* 用于测试任务调度，暂时未加临界区，当三个任务都加入到数组后开启调度器 */
static void addTaskToReadyArray( TCB_t * tcb ) {
    if ( taskNum > 2 ) {
        return
    }

    taskArrayForTest[taskNum++] = tcb
}