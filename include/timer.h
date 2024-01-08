#ifndef TIMER_H
#define TIMER_H

#include "list.h"
#include "task.h"
#include "coreDef.h"
#include "WillingOSConfig.h"
#include "memory.h"

typedef void * TimerCmdHandle_t;

typedef  uint8_t TimerMod_t;
#define TIMER_MOD_ONCE (0U)
#define TIMER_MOD_REPEAT (1U)


extern List_t timerCmdList;

void timerTask(void * param);

TimerCmdHandle_t createTimer_ms(
    uint16_t id,
    uint32_t waitTime, // ????,??ms
    TimerMod_t timerMod,
    TimerCmdFunc_t timerCommand,
    void * param
);

#endif
