#ifndef TASK_H
#define TASK_H


/* 任务句柄，可以用于删除任务 */
typedef void * TaskHandle_t;

#define SCHEDULER_STATE_WAITING      ( ( Base_t ) 0 ) /* 未开始调度 */
#define SCHEDULER_STATE_SUSPENDED    ( ( Base_t ) 1 )
#define SCHEDULER_STATE_RUNNING      ( ( Base_t ) 2 )


Base_t sysTickService( void );
Base_t getSchedulerState( void );
void taskSwitchContext( void );
void OSStart(void);
void OSStop(void);

Long_t createTask( TaskFunc_t taskFunc, /* 任务函数 */ 
                    const char* const taskName,  /* 任务名称 */ 
                    const uint32_t stackSize, /* 栈大小 */ 
                    void * const params, /* 传递给taskFunc的参数 */ 
                    ULong_t priority, /* 优先级 */ 
                    TaskHandle_t * const taskHandler /* 任务句柄 */ );

// #define ENTER_CRITICAL_SECTION() enterCriticalSection()
// #define EXIT_CRITICAL_SECTION()  exitCriticalSection()

#endif