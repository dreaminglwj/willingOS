#include <stdint.h>
#include <string.h>

#include "coreDef.h"
#include "task.h"
#include "willing.h"
#include "WillingOSConfig.h"
#include "memory.h"
#include "protable.h"
#include "list.h"
#include "timer.h"

typedef struct taskControlBlock
{
    volatile Stack_t *stackTop; /* 栈顶指针，指向最后一个入栈的元素 */
    Stack_t *stack;             /* 栈底指针，指向栈的起始位置 */
    ULong_t priority;

    /* delay相关 */
    uint32_t delayExpireAt;   /* 延时结束时间，为n个tickCount，0表示没有延时 */
    uint8_t tickCountSession; /* tickCount会话 */

    char taskName[MAX_TASK_NAME_LEN]; /* 任务名称 */
} TCB;

/* 重新定义一个tcb变量，供编程使用 */
typedef TCB TCB_t;

// todo：因为在port中用到了这些变量，暂时只能导出去，等调试通过了再来整理，还原成static
/*static*/ volatile Base_t schedulerRunning = wFALSE;
/*static*/ volatile Base_t schedulerSuspended = 0;
/*static*/ volatile uint32_t tickCount = 0U;
/*static*/ volatile uint8_t tickCountSession = 0U;

/* 任务队列 */

// TCB_t * taskArrayForTest[MAX_TASK_NUM];
// uint16_t taskNum = 0;
// TCB_t * volatile currentTCB = NULL;
// uint16_t currentTaskIndex =0;

List_t sysReadyTaskList;
List_t sysSuspendTaskList;
ListItem_t *currentTaskItem = NULL;
TCB_t *volatile currentTCB = NULL;
ListItem_t *nextTaskItem = NULL; /* 在sysTickService中确定要不要调度的时候就决定好下一步调度到哪个任务
                                                       避免在switch的时候再来遍历链表。
                                                       todo：测试会不会产生调度之前又发生变化的问题 */
TaskHandle_t idleTaskHandler;
TaskHandle_t timerTaskHandler;
ListItem_t *timerTaskItem;

static void initTask(TaskFunc_t taskFunc,
                     const char *const taskName,
                     const uint32_t stackSize,
                     void *const params,
                     UBase_t priority,
                     TCB_t *tcb,
                     TaskHandle_t *const taskHandler);

/* 用于测试任务调度，暂时未加临界区，当三个任务都加入到数组后开启调度器 */
static void addTaskToReadyArray(TCB_t *tcb);

Long_t createTask(TaskFunc_t taskFunc,        /* 任务函数 */
                  const char *const taskName, /* 任务名称 */
                  const uint32_t stackSize,   /* 栈大小 */
                  void *const params,         /* 传递给taskFunc的参数 */
                  ULong_t priority,           /* 优先级 */
                  TaskHandle_t *const taskHandler /* 任务句柄 */)
{
    TCB_t *tcb = NULL;
    Long_t rlt = -1;

/* 堆栈向上增长（正向生长，51单片机）：从低地址（0x00）向高地址(0xFF)增长
堆栈向下增长（逆向生长，80x86和cotex-M3）：从高地址(0xFF)向低地址（0x00）增长 */
/* 如果栈向下生长，先给stack分配空间再给tcb分配空间，防止栈增长覆盖tcb内存；
如果栈向上生长，先给tcb分配空间，再给stack分配空间 */
#if (STACK_GROWTH == STACK_GROWTH_DOWN)
    {
        Stack_t *stack;

        stack = (Stack_t *)willingMalloc(((size_t)stackSize) * sizeof(Stack_t));

        if (stack != NULL)
        {
            tcb = (TCB_t *)willingMalloc(sizeof(TCB_t));

            if (tcb != NULL)
            {
                tcb->stack = stack;
            }
            else
            {
                willingFree(stack);
            }
        }
    }
#else
    { /* STACK_GROWTH */
    }
#endif /* STACK_GROWTH */

    if (tcb != NULL)
    {
        initTask(taskFunc, taskName, stackSize, params, priority, tcb, taskHandler);
        addTaskToReadyArray(tcb);
        rlt = wPASS;
    }
    else
    {
        rlt = errMEMORY_ALLOCATE_FAIL;
    }
    return rlt;
}

static void initTask(TaskFunc_t taskFunc,
                     const char *const taskName,
                     const uint32_t stackSize,
                     void *const params,
                     UBase_t priority,
                     TCB_t *tcb,
                     TaskHandle_t *const taskHandler)
{
    Stack_t *topOfStack = NULL;
    UBase_t i = 0;

#if (STACK_GROWTH == STACK_GROWTH_DOWN)
    {
        topOfStack = tcb->stack + (stackSize - (uint32_t)1);
        topOfStack = (Stack_t *)(((willingPOINTER_SIZE_TYPE)topOfStack) & (~((willingPOINTER_SIZE_TYPE)BYTE_ALIGNMENT_MASK)));

        // 检查溢出
    }
#else
    { /* STACK_GROWTH */
    }
#endif /* STACK_GROWTH */

    /* 拷贝taskName */
    for (i = (UBase_t)0; i < (UBase_t)MAX_TASK_NAME_LEN; i++)
    {
        tcb->taskName[i] = taskName[i];
        if (taskName[i] == 0x00)
        {
            break;
        }
        else
        {
            //
        }
    }

    tcb->taskName[MAX_TASK_NAME_LEN - 1] = 0x00;

    /* 赋值priority */
    if (priority >= (UBase_t)MAX_PRIORITY_NUM)
    {
        tcb->priority = (UBase_t)MAX_PRIORITY_NUM - (UBase_t)1U;
    }
    else
    {
        tcb->priority = priority;
    }

    tcb->delayExpireAt = DELAY_VALUE_NO_DELAY;
    tcb->tickCountSession = tickCountSession;

    tcb->stackTop = initStack(topOfStack, taskFunc, params);

    if ((void *)taskHandler != NULL)
    {
        *taskHandler = (TaskHandle_t)tcb;
    }
    else
    {
        /* 错误处理 */
    }
}

/* 用于测试任务调度，暂时未加临界区，当三个任务都加入到数组后开启调度器 */
static void addTaskToReadyArray(TCB_t *tcb)
{
    UBase_t nums = 0;
    ListItem_t *item = (ListItem_t *)willingMalloc(sizeof(ListItem_t));
    item->tcbWith = tcb;
    item->listWith = &sysReadyTaskList;

    nums = insertWillingList_Behind(&sysReadyTaskList, sysReadyTaskList.tail, item);
    willingAssert(nums);

    if (currentTCB == NULL)
    {
        currentTaskItem = item;
        currentTCB = tcb;
    }
}

/*
    系统时钟服务函数，
    调度算法函数在这个函数中调用
*/
Base_t sysTickService(void)
{
    Base_t needSwitchCtx = wFALSE;
    ListItem_t *itemToRun = NULL;
    ListItem_t *itemItr = sysReadyTaskList.head;
    Base_t currentTCBAlive = wFALSE; // 当前任务是否在就绪表中

    willingAssert(sysReadyTaskList.itemNum);

    /* if (就绪表只有一个任务) {
          if ( 就绪表中的任务 不等于 当前任务 ) {
               说明当前任务被挂起了，所以需要进行调度，切换到idle任务
        } else {
               不需要进行调度
        }
       } else {  就绪表中有多个任务的时候
        if ( 当前任务不在就绪表中 ) {
             切换
        } else { 当前任务在就绪表中
           if ( 有优先级大于等于当前任务的任务存在 ) {
                            切换
           } else {
                不切换
           }
          }
     }


       优化一下：
         if ( 当前任务在就绪表中 ) {
             if ( 存在优先级  大于等于  当前任务的任务 ) {
                     切换
               }
       } else { 当前任务不在就绪表中，这种情况一般是因为调用了sleep函数
             切换
       }

         其他：不切换
    */

    do
    {
        TCB_t *tcbItr = (TCB_t *)(itemItr->tcbWith);

        if (tcbItr == currentTCB)
        {
            currentTCBAlive = wTRUE;
        }
        else
        {
            if (itemToRun == NULL && tcbItr->priority >= currentTCB->priority)
            {
                itemToRun = itemItr;
            }
        }

        //  todo：对于多次tick的都没有被调用的任务，进行优先级提升

        itemItr = itemItr->next;
    } while (itemItr != NULL);

    if (currentTCBAlive == wTRUE)
    {
        if (itemToRun != NULL)
        { // todo： 这里适配了sleep的情况？对于timer的情况，这里判断不对，因为timer没有被挂起，所以所alive的，进来之后itemToRun为空，导致认为不需要调度，函数退出后assert失败，所以可能不需要assert
            nextTaskItem = itemToRun;
            needSwitchCtx = wTRUE;
        }
    }
    else
    {
        nextTaskItem = sysReadyTaskList.head;
        needSwitchCtx = wTRUE;
    }

    return needSwitchCtx;
}

Base_t getSchedulerState(void)
{
    Base_t rlt = SCHEDULER_STATE_RUNNING;

    /* todo: 完成具体实现 */
    if (schedulerRunning == wFALSE)
    {
        rlt = SCHEDULER_STATE_WAITING;
    }
    else
    {
        if (schedulerSuspended == wFALSE)
        {
            if (sysReadyTaskList.itemNum == 0)
            { // todo: 确认这样做是否可行，没有任务在跑系统是否能持续运转？或者一直在跑当前要停掉的任务？极大可能这段代码要删掉，改成插入一个idle任务
                rlt = SCHEDULER_STATE_SUSPENDED;
            }
            else
            {
                rlt = SCHEDULER_STATE_RUNNING;
            }
        }
        else
        {
            rlt = SCHEDULER_STATE_SUSPENDED;
        }
    }

    return rlt;
}

void taskSwitchContext(void)
{
    // 将curTcb指向下一个就绪的任务控制块
    willingAssert(sysReadyTaskList.itemNum > 0);

    if (nextTaskItem != NULL)
    {
        currentTaskItem = nextTaskItem;
        nextTaskItem = NULL;
        currentTCB = currentTaskItem->tcbWith;
    }

    // currentItem = getWillingListNextItem_Circle( &readyTaskList, currentItem );
    // willingAssert( currentItem );

    // currentTCB = currentItem->tcbWith;
}

void idleTask(void *param)
{
    while (1)
    {
        //		int i = 0; // 用于调试
        //		i=1;
        ;
    }
}

void OSStart(void)
{

    DISABLE_INTERRUPTS();

    createTask((TaskFunc_t)timerTask,
               (const char *)"timerTask",
               (uint32_t)20,
               (void *)NULL,
               (UBase_t)MAX_PRIORITY_VALUE,
               (TaskHandle_t *)timerTaskHandler);

    timerTaskItem = sysReadyTaskList.tail;  // 因为系统中只有一个timerTask，为了避免循环查找，直接记录其值就好

    // idleTask，由于优先级比较低，所以当有其他任务的时候，idletask是不会被执行的
    createTask((TaskFunc_t)idleTask,
               (const char *)"idleTask",
               (uint32_t)20,
               (void *)NULL,
               (UBase_t)MIN_PRIORITY_VALUE,
               (TaskHandle_t *)idleTaskHandler);

    schedulerRunning = wTRUE;
    tickCount = 0U;

    if (startWillingScheduler() != wFALSE)
    {
    }
    else
    {
    }
}

void reassignTimerTaskExpireTime(uint32_t expireAt, uint8_t tickSession)
{
    if (timerTaskHandler != NULL)
    { // 有可能在创建timer的时候timerTask还没有启动，所以需要在创建timerTask的时候去检查时间最小的timer
        TCB_t *timerTCB = (TCB_t *)timerTaskHandler;
        timerTaskItem->sortValue = expireAt;
        timerTaskItem->tickCountSession = tickSession;

        timerTCB->delayExpireAt = expireAt;
        timerTCB->tickCountSession = tickSession;
    }
}

void OSStop(void)
{
}

uint8_t calculateTickCount_ms(uint32_t nms, uint32_t *ticksWait)
{
    uint32_t ticks = (nms * SYS_TICK_RATE) / 1000;
    uint8_t session = tickCountSession;
    if (ticksWait == NULL)
    {
        return 0;
    }

    *ticksWait = tickCount + ticks;
    if (*ticksWait < tickCount)
    {
        session = !tickCountSession;
    }

    return session;
}

// 单位ms
void willingSleep_ms(uint32_t n)
{
    // uint32_t ticks = ( n * SYS_TICK_RATE ) / 1000; // n / ( 1000 / SYS_TICK_RATE )
    uint32_t ticks = 0;
    uint8_t session = 0;
    // UBase_t rlt = 0;

    // suspendScheduler();

    session = calculateTickCount_ms(n, &ticks);

    willingSleep_ticks(ticks, session);

    //     // currentTCB->delayExpireAt = tickCount + ticks;
    //     // if ( currentTCB->delayExpireAt < tickCount ) {
    //     //     currentTCB->tickCountSession = !tickCountSession;
    //     // } else {
    //     //     currentTCB->tickCountSession = tickCountSession;
    //     // }

    //     currentTaskItem->sortValue = currentTCB->delayExpireAt;
    //     currentTaskItem->tickCountSession = currentTCB->tickCountSession;

    // //  todo: 存在风险，可能会跳过下一个任务，要去看看switch函数怎么实现的，什么时候调用的
    // // 当就绪列表中没有任务的时候应该插入一个idleTask
    // //    ListItem_t * next = getWillingListNextItem_Circle( &readyTaskList, currentTaskItem );
    // //    if ( next == NULL ) {
    // //       /* todo: 插入一个idleTask，或者在初始化的时候插入一个，但是要保证这个任务不占cpu时间 */
    // //    }

    //     removeWillingListItem( &readyTaskList, currentTaskItem );
    //     rlt = insertWillingList_SortASC( &suspendTaskList,  currentTaskItem );
    //     willingAssert(rlt);
    //     // currentTaskItem = next;

    // resumeScheduler();
}

void willingSleep_ticks(int32_t ticks, uint8_t tickSession)
{
    UBase_t rlt = 0;

    suspendScheduler();

    currentTCB->tickCountSession = tickSession;
    currentTCB->delayExpireAt = ticks;

    currentTaskItem->sortValue = currentTCB->delayExpireAt;
    currentTaskItem->tickCountSession = currentTCB->tickCountSession;

    removeWillingListItem(&sysReadyTaskList, currentTaskItem);
    rlt = insertWillingList_SortASC(&sysSuspendTaskList, currentTaskItem);
    willingAssert(rlt);

    resumeScheduler();
}

void suspendScheduler(void)
{
    ++schedulerSuspended;
}

UBase_t resumeScheduler(void)
{
    if (schedulerSuspended > 0)
    {
        --schedulerSuspended;
    }

    if (schedulerSuspended == 0)
    { // 开启调度，保证函数可重入
        //  产生一个PendSV中断
        // willingAssert( sysTickService() == wTRUE ); // todo： 用assert会导致timer那边出问题,todo:调试sleep
        if (sysTickService() == wTRUE)
        {
            NVIC_INTERRUPUT_CTRL_REG = NVIC_PENDSV_SET_BIT;
        }
    }

    return schedulerSuspended;
}

void processDelay(void)
{
    ListItem_t *item = getWillingListHeadItem(&sysSuspendTaskList);
    TCB_t *tcb = NULL;

    while (item != NULL)
    {
        tcb = item->tcbWith;
        if (tcb->tickCountSession == tickCountSession && tcb->delayExpireAt <= tickCount)
        {
            removeWillingListItem(&sysSuspendTaskList, item);
            insertWillingList_Head(&sysReadyTaskList, item);
        }

        item = getWillingListNextItem(&sysSuspendTaskList, item);
    }
}

void initKernel(void)
{
    sysReadyTaskList.head = NULL;
    sysReadyTaskList.tail = NULL;
    sysReadyTaskList.itemNum = 0;

    sysSuspendTaskList.head = NULL;
    sysSuspendTaskList.tail = NULL;
    sysSuspendTaskList.itemNum = 0;

    tickCountSession = 0;
    tickCount = 0;

    timerTaskItem = NULL;
}
