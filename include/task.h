#ifndef TASK_H
#define TASK_H


/* 任务句柄，可以用于删除任务 */
typedef void * TaskHandle_t;

#define SCHEDULER_STATE_WAITING      ( ( Base_t ) 0 ) /* 未开始调度 */
#define SCHEDULER_STATE_SUSPENDED    ( ( Base_t ) 1 )
#define SCHEDULER_STATE_RUNNING      ( ( Base_t ) 2 )


Base_t sysTickService( void );
Base_t getSchedulerState( void );

#endif