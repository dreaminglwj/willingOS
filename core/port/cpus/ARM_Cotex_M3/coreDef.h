#ifndef CORE_DEF_H
#define CORE_DEF_H

#ifdef __cplusplus
extern "c" {
#endif


/* 基础类型定义 */
#define willingCHAR		char  /* wl: willing */
#define willingFLOAT	float
#define willingDOUBLE	double
#define willingLONG		long
#define willingSHORT	short
#define willingSTACK	uint32_t
#define willingBASE	    long

typedef willingSTACK Stack_t;
typedef long Long_t;
typedef unsigned long ULong_t;
typedef long Base_t;
typedef unsigned long UBase_t;

/* cpu架构相关变量 */
#define STACK_GROWTH_UP (1)   
#define STACK_GROWTH_DOWN (-1)
#define TICK_PERIOD_MS  ()
#define BYTE_ALIGNMENT   8

#define STACK_GROWTH STACK_GROWTH_DOWN

/* 任务相关 */
typedef void (*TaskFunc_t)(void *);


#ifdef __cplusplus
}
#endif


#endif