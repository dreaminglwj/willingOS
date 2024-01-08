#include "timer.h"

typedef struct timerCommand
{
    TimerCmdFunc_t timerCmd; // 定时任务函数
    TimerMod_t timerMod;

    uint32_t timeOutAt; /* 超时时间 */
    uint8_t   tickCountSession;
    uint32_t timeOutSet; /* 超时间隔时间 */
	
	  void * param;

    uint16_t timerId;
} TimerCommand_t ;

List_t timerCmdList;


void readjustTimerTaskWaitTime( uint32_t timeOutAt, uint32_t tickCountSession );
void processTimerCmd( void );

TimerCmdHandle_t createTimer_ms(
    uint16_t id,
    uint32_t waitTime, // 延迟时间，单位ms
    TimerMod_t timerMod,
    TimerCmdFunc_t timerCommand,
    void * param
) {
    // 计算tickCount

    // 初始化cmd
    ListItem_t * cmdItem = (ListItem_t*) willingMalloc( sizeof( ListItem_t ) );
    TimerCommand_t *timerCmd = ( TimerCommand_t * ) willingMalloc( sizeof( TimerCommand_t ) );

    timerCmd->tickCountSession = calculateTickCount_ms( waitTime, &( timerCmd->timeOutAt ) );
    timerCmd->timeOutSet = ( waitTime * SYS_TICK_RATE ) / 1000; 
    timerCmd->timerCmd = timerCommand;
    timerCmd->timerMod = timerMod;
    timerCmd->timerId = id;
	  timerCmd->param = param;

    cmdItem->tcbWith = timerCmd; // 借用tcbWith来存储cmd
    // cmdItem->listWith = &timerCmdList;
    cmdItem->sortValue = timerCmd->timeOutAt;
    cmdItem->tickCountSession = timerCmd->tickCountSession;

    // 将cmd插入timerCmdList，按照tickCountSession和timeOutAt排序
    willingAssert( insertWillingList_SortASC( &timerCmdList, cmdItem ) );

    // 调整timerTask的sleep值，比如当前在等一个10s的cmd，但是插入了一个1s的cmd，不可能等到10s后再执行
    if ( cmdItem == timerCmdList.head ) { // 如果新插入的cmd在第一个，说明延时值是最小的，需要调整timerTask的sleep值
        readjustTimerTaskWaitTime( timerCmd->timeOutAt, timerCmd->tickCountSession  );
    }
		
		return (TimerCmdHandle_t)timerCmd;
}

void timerTask(void * param) {
    // 查找超时的cmd进行调用
    // 如果是循环调用的cmd，重设延时值，从队列删除cmd，重新插入（重排序）
    // 查找下一个cmd，调整timerTask的sleep值，当前timerTask正在执行，因此sleep就可以了

    ListItem_t * cmdItem = timerCmdList.head;
    TimerCommand_t * timerCmd = NULL; 
    for(;;) {
        if( cmdItem == NULL ) {
            willingSleep_ms( TIMER_IDLE_DELAY_VALUE );
        } else {
            willingAssert( cmdItem );
            timerCmd = (TimerCommand_t*)(cmdItem->tcbWith);
					if ( timerCmd->tickCountSession == tickCountSession && timerCmd->timeOutAt <= tickCount ) {
							 suspendScheduler();

            timerCmd->timerCmd( timerCmd->param );
            removeWillingListItem(&timerCmdList, cmdItem );
            if ( timerCmd->timerMod == TIMER_MOD_REPEAT ) {
                // 直接在timerCmd上次超时上加，保证每次执行的interval是一样的，但是当超时过短（比任务执行时间短）时有可能产生任务堆积
                // todo: 考虑改成基于系统的tickCount和tickCountSession计算
                uint32_t timeOutAt = timerCmd->timeOutAt + timerCmd->timeOutSet;
                if ( timeOutAt < timerCmd->timeOutAt ) {
                    timerCmd->tickCountSession = !(timerCmd->tickCountSession);
                    cmdItem->tickCountSession = timerCmd->tickCountSession;
                }
								   timerCmd->timeOutAt = timeOutAt;
                   cmdItem->sortValue = timeOutAt;

                willingAssert( insertWillingList_SortASC( &timerCmdList, cmdItem ) );
            }

            resumeScheduler();
						cmdItem = timerCmdList.head;
						} else {
						   willingSleep_ticks( timerCmd->timeOutAt, timerCmd->tickCountSession );
						}



            
        }
    }
}

void processTimerCmd( void ) {
    ListItem_t * cmdItem = timerCmdList.head;
    TimerCommand_t * timerCmd = NULL; 

    while( cmdItem != NULL ) {
        timerCmd = (TimerCommand_t *) cmdItem->tcbWith;
        if ( timerCmd->tickCountSession == tickCountSession && timerCmd->timeOutAt >= tickCount ) {
            // 执行
        } else { // 调整sleep值
           willingSleep_ticks( timerCmd->timeOutAt, timerCmd->tickCountSession );
        }
    }
}


void readjustTimerTaskWaitTime( uint32_t timeOutAt, uint32_t tickSession ) {
    reassignTimerTaskExpireTime( timeOutAt, tickSession );
}
  

