#ifndef TASK_H
#define TASK_H

#include "coreDef.h"


/* 任务句柄，可以用于删除任务 */
typedef void * TaskHandle_t;

#define SCHEDULER_STATE_SUSPENDED      ( ( Base_t ) 0 ) /* 未开始调度 */
#define SCHEDULER_STATE_WAITING    ( ( Base_t ) 1 )
#define SCHEDULER_STATE_RUNNING      ( ( Base_t ) 2 )

extern volatile Base_t schedulerRunning;
extern volatile Base_t schedulerSuspended;
extern volatile uint32_t tickCount;
extern volatile uint8_t  tickCountSession;


Base_t sysTickService( void );
Base_t getSchedulerState( void );
void taskSwitchContext( void );
void OSStart(void);
void OSStop(void);
void willingSleep_ms( uint32_t n ); // 单位ms
void suspendScheduler(void);
UBase_t resumeScheduler(void);
void processDelay( void );
void initKernel( void );

Long_t createTask( TaskFunc_t taskFunc, /* 任务函数 */ 
                    const char* const taskName,  /* 任务名称 */ 
                    const uint32_t stackSize, /* 栈大小 */ 
                    void * const params, /* 传递给taskFunc的参数 */ 
                    ULong_t priority, /* 优先级 */ 
                    TaskHandle_t * const taskHandler /* 任务句柄 */ );

// #define ENTER_CRITICAL_SECTION() enterCriticalSection()
// #define EXIT_CRITICAL_SECTION()  exitCriticalSection()

#endif
