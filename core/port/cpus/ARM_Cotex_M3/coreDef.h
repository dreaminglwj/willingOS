#ifndef CORE_DEF_H
#define CORE_DEF_H

#ifdef __cplusplus
extern "c" {
#endif

#include <stdint.h>
#include "willingOSConfig.h"
#include "stdio.h"
//#include "core_cm3.h"



#ifdef __cplusplus
  #define     __I     volatile                /**/ defines 'read only' permissions      */
#else
  #define     __I     volatile const          /* defines 'read only' permissions      */
#endif
#define     __O     volatile                  /* defines 'write only' permissions     */
#define     __IO    volatile                  /* defines 'read / write' permissions   */


#if defined ( __CC_ARM   )
  #define __ASM            __asm                                      /*!< asm keyword for ARM Compiler          */
  #define __INLINE         __inline                                   /*!< inline keyword for ARM Compiler       */

#elif defined ( __ICCARM__ )
  #define __ASM           __asm                                       /*!< asm keyword for IAR Compiler          */
  #define __INLINE        inline                                      /*!< inline keyword for IAR Compiler. Only avaiable in High optimization mode! */

#elif defined   (  __GNUC__  )
  #define __ASM            __asm                                      /*!< asm keyword for GNU Compiler          */
  #define __INLINE         inline                                     /*!< inline keyword for GNU Compiler       */

#elif defined   (  __TASKING__  )
  #define __ASM            __asm                                      /*!< asm keyword for TASKING Compiler      */
  #define __INLINE         inline                                     /*!< inline keyword for TASKING Compiler   */

#endif

/******************************************************************
            中断相关配置                                                
******************************************************************/
#ifndef __CORTEX_M3_INTERRUPT_PRIORITY_BITS 
    #define __CORTEX_M3_INTERRUPT_PRIORITY_BITS 8
#endif

#ifndef __NVIC_PRIORITY_BITS
    #define __NVIC_PRIORITY_BITS 4 /* nvic优先级标准定义 */
#endif


/* 用于检查中断优先级的变量定义 */
#define FIRST_USER_INTERRUPT_NUMBER		( 16 )
#define NVIC_IP_REGISTERS_OFFSET_16 	( 0xE000E3F0 )
#define AIRCR_REG						( * ( ( volatile uint32_t * ) 0xE000ED0C ) )
#define MAX_8_BIT_VALUE					( ( uint8_t ) 0xff )
#define TOP_BIT_OF_BYTE					( ( uint8_t ) 0x80 )
#define MAX_PRIGROUP_BITS				( ( uint8_t ) 7 )
#define PRIORITY_GROUP_MASK				( 0x07UL << 8UL )
#define PRIGROUP_SHIFT					( 8UL )


/******************************************************************
            systick相关配置                                                 
******************************************************************/
/* cortex-m3地址映射关系 */
#define SYS_CTRL_BASE       (0xE000E000) /* system ctrl space base address */
//#define ITM_BASE            (0xE0000000)                              /* ITM:指令跟踪单元                   */

#define SYS_TICK_BASE       (SYS_CTRL_BASE + 0X10) /* 系统节拍控制基地址 */


typedef struct
{
  __IO uint32_t CtrlReg;                         
  __IO uint32_t ReloadReg;                         
  __IO uint32_t CurValueReg;                          
  __I  uint32_t SysTickCalibrationReg;                       
} SysTick_t;

#define SysTickCtrl ((SysTick_t*) SYS_TICK_BASE) /* sysTick寄存器组指针 */


//#define SysTick_CTRL_COUNTFLAG_Pos         16                                            
// #define SysTick_CTRL_COUNTFLAG_Mask         (1ul << SysTick_CTRL_COUNTFLAG_Pos)          

// //#define SysTick_CTRL_CLKSOURCE_Pos          2                                             
// #define SysTick_CTRL_CLKSOURCE_Mask         (1ul << SysTick_CTRL_CLKSOURCE_Pos)         

// #define SysTick_CTRL_TICK_INTERRUPT_Pos     1                                           
// #define SysTick_CTRL_TICK_INTERRUPT_Mask    (1ul << SysTick_CTRL_TICK_INTERRUPT_Pos)    

// //#define SysTick_CTRL_ENABLE_Pos             0                                           
// #define SysTick_CTRL_ENABLE_Mask            (1ul << SysTick_CTRL_ENABLE_Pos)            

// /* SysTick Reload Register Definitions */
// #define SysTick_RELOAD_RELOAD_Pos             0                                         
// #define SysTick_RELOAD_RELOAD_Mask            (0xFFFFFFul << SysTick_RELOAD_RELOAD_Pos) 

// /* SysTick Current Register Definitions */
// #define SysTick_CUR_VALUE_Pos                 0                                         
// #define SysTick_CUR_VALUE_Mask                (0xFFFFFFul << SysTick_CUR_VALUE_Pos)     

// /* SysTick Calibration Register Definitions */
// #define SysTick_CALIBRATION_NOREF_Pos            31                                     
// #define SysTick_CALIBRATION_NOREF_Mask            (1ul << SysTick_CALIBRATION_NOREF_Pos)

// #define SysTick_CALIBRATION_SKEW_Pos             30                                     
// #define SysTick_CALIBRATION_SKEW_Mask             (1ul << SysTick_CALIBRATION_SKEW_Pos) 

// #define SysTick_CALIBRATION_TENMS_Pos             0                                     
// #define SysTick_CALIBRATION_TENMS_Mask            (0xFFFFFFul << SysTick_CUR_VALUE_Pos) 





/******************************************************************
            时钟相关配置                                               
******************************************************************/

#define SYSCLK_FREQUENCE_72MHz  72000000

#ifdef SYSCLK_FREQUENCE_72MHz
extern  uint32_t SysClockFrequence;
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

static UBase_t criticalNesting = 0xaaaaaaaa;

/*
    系统控制块（scb:system control block）的内存映射结构
*/
typedef struct
{
  __I  uint32_t CPUID;                        /*!< Offset: 0x00  CPU ID Base Register                                  */
  __IO uint32_t ICSR;                         /*!< Offset: 0x04  Interrupt Control State Register                      */
  __IO uint32_t VTOR;                         /*!< Offset: 0x08  Vector Table Offset Register                          */
  __IO uint32_t AIRCR;                        /*!< Offset: 0x0C  Application Interrupt / Reset Control Register        */
  __IO uint32_t SCR;                          /*!< Offset: 0x10  System Control Register                               */
  __IO uint32_t CCR;                          /*!< Offset: 0x14  Configuration Control Register                        */
  __IO uint8_t  SHP[12];                      /*!< Offset: 0x18  System Handlers Priority Registers (4-7, 8-11, 12-15) */
  __IO uint32_t SHCSR;                        /*!< Offset: 0x24  System Handler Control and State Register             */
  __IO uint32_t CFSR;                         /*!< Offset: 0x28  Configurable Fault Status Register                    */
  __IO uint32_t HFSR;                         /*!< Offset: 0x2C  Hard Fault Status Register                            */
  __IO uint32_t DFSR;                         /*!< Offset: 0x30  Debug Fault Status Register                           */
  __IO uint32_t MMFAR;                        /*!< Offset: 0x34  Mem Manage Address Register                           */
  __IO uint32_t BFAR;                         /*!< Offset: 0x38  Bus Fault Address Register                            */
  __IO uint32_t AFSR;                         /*!< Offset: 0x3C  Auxiliary Fault Status Register                       */
  __I  uint32_t PFR[2];                       /*!< Offset: 0x40  Processor Feature Register                            */
  __I  uint32_t DFR;                          /*!< Offset: 0x48  Debug Feature Register                                */
  __I  uint32_t ADR;                          /*!< Offset: 0x4C  Auxiliary Feature Register                            */
  __I  uint32_t MMFR[4];                      /*!< Offset: 0x50  Memory Model Feature Register                         */
  __I  uint32_t ISAR[5];                      /*!< Offset: 0x60  ISA Feature Register                                  */
} SysCtrlBlock_t;  


#define NVIC_PriorityGroup_0         ((uint32_t)0x700) /*!< 0 bits for pre-emption priority
                                                            4 bits for subpriority */
#define NVIC_PriorityGroup_1         ((uint32_t)0x600) /*!< 1 bits for pre-emption priority
                                                            3 bits for subpriority */
#define NVIC_PriorityGroup_2         ((uint32_t)0x500) /*!< 2 bits for pre-emption priority
                                                            2 bits for subpriority */
#define NVIC_PriorityGroup_3         ((uint32_t)0x400) /*!< 3 bits for pre-emption priority
                                                            1 bits for subpriority */
#define NVIC_PriorityGroup_4         ((uint32_t)0x300) /*!< 4 bits for pre-emption priority
                                                            0 bits for subpriority */

#define IS_NVIC_PRIORITY_GROUP(GROUP) (((GROUP) == NVIC_PriorityGroup_0) || \
                                       ((GROUP) == NVIC_PriorityGroup_1) || \
                                       ((GROUP) == NVIC_PriorityGroup_2) || \
                                       ((GROUP) == NVIC_PriorityGroup_3) || \
                                       ((GROUP) == NVIC_PriorityGroup_4))

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

#define SYS_CTRL_BLOCK_BASE (SYS_CTRL_BASE + 0x0D00)
#define SysCtrlBlock ((SysCtrlBlock_t *) SYS_CTRL_BLOCK_BASE )


/* scheduler 相关定义 */
#define NVIC_INTERRUPUT_CTRL_REG (*( (volatile uint32_t *) 0xE000ED04 ))
#define NVIC_PENDSV_SET_BIT ( 1UL << 28UL )


/* 函数 */
#define willingINLINE __INLINE

#ifndef willingFORCE_INLINE 
   #define willingFORCE_INLINE __forceinline
#endif

extern uint32_t setInterruptMaskFromISR( void );
extern void clearInterruptMaskFromISR( uint32_t mask );

extern void enterCriticalSection( void );
extern void exitCriticalSection( void );

#define SET_INTERRUPT_MASK_FROM_ISR setInterruptMaskFromISR()
#define CLEAR_INTERRUPT_MASK_FROM_ISR(x)  clearInterruptMaskFromISR( x )

static willingFORCE_INLINE void maximizeBasePriority( void ) {
    uint32_t maxPriority = MAX_SYS_INTERRUPT_PRIORITY;
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


/* 临界区相关定义 */
//static willingINLINE void __enable_irq()               { __ASM volatile ("cpsie i"); }
//static willingINLINE void __disable_irq()              { __ASM volatile ("cpsid i"); }

//#define DISABLE_INTERRUPTS __disable_irq()
//#define ENABLE_INTERRUPTS __enable_irq() 

#define DISABLE_INTERRUPTS()				maximizeBasePriority()
#define ENABLE_INTERRUPTS()					clearBasePriorityFromISR()

#define ENTER_CRITICAL_SECTION() enterCriticalSection()
#define EXIT_CRITICAL_SECTION()  exitCriticalSection()


/* 断言 */
#define assrtFunc(char,int) printf("Error:%s,%d\r\n",char,int)
#define willingAssert(x) if((x)==0) assrtFunc(__FILE__,__LINE__)

#ifdef __cplusplus
}
#endif


#endif
