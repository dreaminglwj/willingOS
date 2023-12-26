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


/* scheduler 相关定义 */
#define NVIC_INTERRUPUT_CTRL_REG (*( (volatile uint32_t *) 0xE000ED04 ))
#define NVIC_PENDSV_SET_BIT ( 1UL << 28UL )


/* 函数 */
#define willingINLINE __inline

#ifndef willingFORCE_INLINE 
   #define willingFORCE_INLINE __forceinline
#endif

extern uint32_t setInterruptMaskFromISR( void );
extern void clearInterruptMaskFromISR( uint32_t mask );

#define SET_INTERRUPT_MASK_FROM_ISR setInterruptMaskFromISR()
#define CLEAR_INTERRUPT_MASK_FROM_ISR(x)  clearInterruptMaskFromISR( x )

static willingFORCE_INLINE void maximizeBasePriority( void ) {
    uint32_t maxPriority = MAX_SYS_INTERRUPT_PRIORITY
    __asm{
        msr basepri, maxPriority  /* msr：将普通寄存器的值写入到特殊寄存器 */
        dsb /* 数据同步屏障，通过对指令流水的管理，确保dsb之前的所有显式数据内存传输指令都已经再内存中读取或写入完成，同时确保任何后续的指令都将再dsb执行之后开始执行 */
        isb /* 指令同步屏障，通过插入isb指令，处理器会将流水线中的指令全部刷新（flush），从而确保之前的指令不会影响后续的指令执行，并且后续指令将从正确的上下文开始重新获取 */
    }
}

static willingFORCE_INLINE void clearBasePriorityFromISR( void ) {
    __asm {
        msr basepri, #0
    }
}


#ifdef __cplusplus
}
#endif


#endif