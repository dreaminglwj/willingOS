#include <stdlib.h>
#include <string.h>

#include "coreDef.h"
#include "task.h"
#include "WillingOSConfig.h"


typedef struct taskControlBlock {
    volatile Stack_t  *pStackTop; /* 栈顶指针 */
    Stack_t *pStack; /* 栈底指针 */
    UBase_t ulPriority;

    char pcTaskName[ MAX_TASK_NAME_LEN ]; /* 任务名称 */
} TCB;


/* 重新定义一个tcb变量，供编程使用 */
typedef TCB TCB_t;