#ifndef CPU_MODULE_PRESENT
#define CPU_MODULE_PRESENT

#include <stdlib.h>

#ifdef __cplusplus
  #define     __I     volatile                /**/ defines 'read only' permissions      */
#else
  #define     __I     volatile const          /* defines 'read only' permissions      */
#endif
#define     __O     volatile                  /* defines 'write only' permissions     */
#define     __IO    volatile                  /* defines 'read / write' permissions   */

/******************************************************************
            中断相关配置                                                
******************************************************************/
#ifndef __CORTEX_M3_INTERRUPT_PRIORITY_BITS 
    #define __CORTEX_M3_INTERRUPT_PRIORITY_BITS 8
#endif

#ifndef __NVIC_PRIORITY_BITS
    #define __NVIC_PRIORITY_BITS 4 /* nvic优先级标准定义 */
#endif


/******************************************************************
            systick相关配置                                                 
******************************************************************/
/* cortex-m3地址映射关系 */
#define SYS_CTRL_BASE       (0xE000E000) /* system ctrl space base address */
#define ITM_BASE            (0xE0000000)                              /* ITM:指令跟踪单元                   */

#define SYS_TICK_BASE       (SYS_CTRL_BASE + 0X10) /* 系统节拍控制基地址 */


typedef struct
{
  __IO uint32_t CtrlReg;                         
  __IO uint32_t ReloadReg;                         
  __IO uint32_t CurValueReg;                          
  __I  uint32_t SysTickCalibrationReg;                       
} SysTick_t;

#define SysTick (*(SysTick_t) SYS_TICK_BASE) /* sysTick寄存器组指针 */


#define SysTick_CTRL_COUNTFLAG_Pos         16                                             /*!< SysTick CTRL: COUNTFLAG Position */
#define SysTick_CTRL_COUNTFLAG_Mask         (1ul << SysTick_CTRL_COUNTFLAG_Pos)            /*!< SysTick CTRL: COUNTFLAG Mask */

#define SysTick_CTRL_CLKSOURCE_Pos          2                                             /*!< SysTick CTRL: CLKSOURCE Position */
#define SysTick_CTRL_CLKSOURCE_Mask         (1ul << SysTick_CTRL_CLKSOURCE_Pos)            /*!< SysTick CTRL: CLKSOURCE Mask */

#define SysTick_CTRL_TICK_INTERRUPT_Pos     1                                             /*!< SysTick CTRL: TICKINT Position */
#define SysTick_CTRL_TICK_INTERRUPT_Mask    (1ul << SysTick_CTRL_INTERRUPT_Pos)              /*!< SysTick CTRL: TICKINT Mask */

#define SysTick_CTRL_ENABLE_Pos             0                                             /*!< SysTick CTRL: ENABLE Position */
#define SysTick_CTRL_ENABLE_Mask            (1ul << SysTick_CTRL_ENABLE_Pos)               /*!< SysTick CTRL: ENABLE Mask */

/* SysTick Reload Register Definitions */
#define SysTick_RELOAD_RELOAD_Pos             0                                             /*!< SysTick LOAD: RELOAD Position */
#define SysTick_RELOAD_RELOAD_Mask            (0xFFFFFFul << SysTick_RELOAD_RELOAD_Pos)        /*!< SysTick LOAD: RELOAD Mask */

/* SysTick Current Register Definitions */
#define SysTick_CUR_VALUE_Pos                 0                                             /*!< SysTick VAL: CURRENT Position */
#define SysTick_CUR_VALUE_Mask                (0xFFFFFFul << SysTick_CUR_VALUE_Pos)        /*!< SysTick VAL: CURRENT Mask */

/* SysTick Calibration Register Definitions */
#define SysTick_CALIBRATION_NOREF_Pos            31                                             /*!< SysTick CALIB: NOREF Position */
#define SysTick_CALIBRATION_NOREF_Mask            (1ul << SysTick_CALIBRATION_NOREF_Pos)               /*!< SysTick CALIB: NOREF Mask */

#define SysTick_CALIBRATION_SKEW_Pos             30                                             /*!< SysTick CALIB: SKEW Position */
#define SysTick_CALIBRATION_SKEW_Mask             (1ul << SysTick_CALIBRATION_SKEW_Pos)                /*!< SysTick CALIB: SKEW Mask */

#define SysTick_CALIBRATION_TENMS_Pos             0                                             /*!< SysTick CALIB: TENMS Position */
#define SysTick_CALIBRATION_TENMS_Mask            (0xFFFFFFul << SysTick_CUR_VALUE_Pos)        /*!< SysTick CALIB: TENMS Mask */





/******************************************************************
            时钟相关配置                                               
******************************************************************/

#define SYSCLK_FREQUENCE_72MHz  72000000

#ifdef SYSCLK_FREQUENCE_72MHz
    uint32_t SysClockFrequence = SYSCLK_FREQUENCE_72MHz;
#endif

// #define  CPU_REG_NVIC_NVIC           (*((CPU_REG32 *)(0xE000E004)))             /* Int Ctrl'er Type Reg.                */
// #define  CPU_REG_NVIC_ST_CTRL        (*((CPU_REG32 *)(0xE000E010)))             /* SysTick Ctrl & Status Reg.           */
// #define  CPU_REG_NVIC_ST_RELOAD      (*((CPU_REG32 *)(0xE000E014)))             /* SysTick Reload      Value Reg.       */
// #define  CPU_REG_NVIC_ST_CURRENT     (*((CPU_REG32 *)(0xE000E018)))             /* SysTick Current     Value Reg.       */
// #define  CPU_REG_NVIC_ST_CAL         (*((CPU_REG32 *)(0xE000E01C)))             /* SysTick Calibration Value Reg.       */

#endif