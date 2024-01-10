#include "timer.h"

#ifdef USE_WILLING_SYS_TIMER

typedef struct timerCommand
{
    TimerCmdFunc_t timerCmd; // 定时任务函数
    TimerMod_t timerMod;

    volatile uint8_t tickCountSession;
    uint16_t timerId;
    volatile uint32_t timeOutAt;  /* 超时时间 */
    volatile uint32_t timeOutIntervalSet; /* 设置的超时间隔时间 */

    void *param;
} TimerCommand_t;

List_t timerCmdList;

void readjustTimerTaskWaitTime(uint32_t timeOutAt, uint32_t tickCountSession);
void processTimerCmd(void);

TimerCmdHandle_t createTimer_ms(
    uint16_t id,
    uint32_t waitTime, // 延迟时间，单位ms
    TimerMod_t timerMod,
    TimerCmdFunc_t timerCommand,
    void *param)
{
    // 计算tickCount

    // 初始化cmd
    ListItem_t *cmdItem = (ListItem_t *)willingMalloc(sizeof(ListItem_t));
    TimerCommand_t *timerCmd = (TimerCommand_t *)willingMalloc(sizeof(TimerCommand_t));

    timerCmd->tickCountSession = calculateTickCount_ms(waitTime, &(timerCmd->timeOutAt));
    timerCmd->timeOutIntervalSet = (waitTime * SYS_TICK_RATE) / 1000;
    timerCmd->timerCmd = timerCommand;
    timerCmd->timerMod = timerMod;
    timerCmd->timerId = id;
    timerCmd->param = param;

    cmdItem->tcbWith = timerCmd; // 借用tcbWith来存储cmd
    // cmdItem->listWith = &timerCmdList;
    cmdItem->sortValue = timerCmd->timeOutAt;
    cmdItem->tickCountSession = timerCmd->tickCountSession;

    // 将cmd插入timerCmdList，按照tickCountSession和timeOutAt排序
    willingAssert(insertWillingList_SortASC(&timerCmdList, cmdItem));

    // 调整timerTask的sleep值，比如当前在等一个10s的cmd，但是插入了一个1s的cmd，不可能等到10s后再执行
    if (cmdItem == timerCmdList.head)
    { // 如果新插入的cmd在第一个，说明延时值是最小的，需要调整timerTask的sleep值
        readjustTimerTaskWaitTime(timerCmd->timeOutAt, timerCmd->tickCountSession);
    }

    return (TimerCmdHandle_t)timerCmd;
}

// todo：bug202401090001：目前存在每次timerTask睡眠后再唤醒timeOutSet值变成TimeOutAt值，导致延时叠加越来越长的问题
// 据调试发现，问题发生在第二次cmd执行后的第二次pendSV执行完，之后进入到for循环的时候
// 为什么需要发生两次pendSV?  第一次在resumeScheduler的时候，第二次在sleep结束
// 那么是否有必要在resume的时候切换一次呢？
// 又为什么会在sleep之后timeOutSet的值会变呢？
// 20240110:今天调试的结果是timeOutSet变成了0，发生位置是pendSVHandler  stmdb r0!, {r4-r11},已明确，是因为stackTop地址溢出，跟timerCmd->timeOutIntervalSet地址重合
// bug原因：因为tcb的stack分配的空间小了（20），而stack中要保存初始化的值，占16word，实际切换的时候使用8word，所以是24word，超出的4个word正好溢出到了timercmd->set上
void timerTask(void *param)
{
    // 查找超时的cmd进行调用
    // 如果是循环调用的cmd，重设延时值，从队列删除cmd，重新插入（重排序）
    // 查找下一个cmd，调整timerTask的sleep值，当前timerTask正在执行，因此sleep就可以了

    ListItem_t *cmdItem = timerCmdList.head;
    TimerCommand_t *timerCmd = NULL;
    for (;;)
    {
        if (cmdItem == NULL)
        {
            willingSleep_ms(TIMER_IDLE_DELAY_VALUE);
        }
        else
        {
            willingAssert(cmdItem);
            timerCmd = (TimerCommand_t *)(cmdItem->tcbWith);

            if (timerCmd->tickCountSession == tickCountSession && timerCmd->timeOutAt <= tickCount)
            {
                suspendScheduler();

                timerCmd->timerCmd(timerCmd->param); // 执行命令

                removeWillingListItem(&timerCmdList, cmdItem);
                if (timerCmd->timerMod == TIMER_MOD_REPEAT)
                {
                    // 直接在timerCmd上次超时上加，保证每次执行的interval是一样的，但是当超时过短（比任务执行时间短）时有可能产生任务堆积
                    // todo: 考虑改成基于系统的tickCount和tickCountSession计算
                    uint32_t timeOutAt = timerCmd->timeOutAt + timerCmd->timeOutIntervalSet;
                    if (timeOutAt < timerCmd->timeOutAt)
                    {
                        timerCmd->tickCountSession = !(timerCmd->tickCountSession);
                        cmdItem->tickCountSession = timerCmd->tickCountSession;
                    }
                    timerCmd->timeOutAt = timeOutAt;
                    cmdItem->sortValue = timeOutAt;

                    willingAssert(insertWillingList_SortASC(&timerCmdList, cmdItem));
                } else {
                    // 释放cmd和item
                }

                resumeScheduler();
                cmdItem = timerCmdList.head; // 每次都拿第一个timerCmd来执行，因为有做排序，定时大的都在后面 todo: 确认是不是这一句写在了resumeScheduler外面导致的问题，因为前面有可能已经进行了上下文切换
                                                                    // 应该不是这个问题，因为当前就绪队列中只有timer和idle两个task，timerTask没有sleep，所以不会发生上下文切换，但是还是测试一下试试看
            }
            else
            {
                willingSleep_ticks(timerCmd->timeOutAt, timerCmd->tickCountSession);
            }
        }
    }
}


void readjustTimerTaskWaitTime(uint32_t timeOutAt, uint32_t tickSession)
{
    reassignTimerTaskExpireTime(timeOutAt, tickSession);
}

#endif