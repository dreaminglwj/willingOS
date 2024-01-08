#ifndef TIMER_H
#define TIMER_H

#include "list.h"
#include "task.h"
#include "coreDef.h"
#include "WillingOSConfig.h"

typedef void * TimerCmdHandle_t;

typedef  uint8_t TimerMod_t;
#define TIMER_MOD_ONCE (0U)
#define TIMER_MOD_REPEAT (1U)


extern List_t timerCmdList;

#endif
