#include "sysTick.h"
#include "cpu.h"
#include "willingOSConfig.h"

void setSysTickClkSource(uint32_t source) {
    if (IsSysTickClkSource(source)) {
        if (source == SysTickClkSourceHCLK) {
            SysTick->CtrlReg |= SysTickClkSourceHCLK;
        } else {
            SysTick->CtrlReg |= SysTickClkSourceHCLKDiv8;
        }
    }
}

/* 初始化系统节拍 */
void initSysTick( uint8_t sysClk ) {
    uint32_t reloadValue = 0;
    setSysTickClkSource( SysTickClkSourceHCLK );
    reloadValue = SysClockFrequence / SYS_TICK_RATE; /* 根据configTICK_RATE_HZ设定溢出时间
												     reload为24位寄存器,最大值:16777216,在72M下,约0.233s */

    // 配置硬件
    SysTick->CtrlReg |= SysTick_CTRL_TICK_INTERRUPT_Mask; /* 开启systick 中断 */
    SysTick->reloadValue = reloadValue; /* 每1/SYS_TICK_RATE秒中断一次 */
    SysTick->CtrlReg |= SysTick_CTRL_ENABLE_Mask; /* 开启SysTick */
}


void delay_ms(uint32_t n){

}


void delay_us(uint32_t n){

}

void delay_xms(uint32_t n){

}

