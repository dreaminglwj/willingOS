#ifndef SYS_TICK_H
#define SYS_TICK_H
#include <stdint.h>

#define SysTickClkSourceHCLKDiv8    ((uint32_t)0xFFFFFFFB) /* AHB clock divided by 8 selected as systick clock source */
#define SysTickClkSourceHCLK        ((uint32_t)0x00000004) /* ANB clock selected as systick clock source */
#define IsSysTickClkSource(source) (((source) == SysTickClkSourceHCLK) || \
                                    ((source) == SysTickClkSourceHCLKDiv8))

void initSysTick(uint8_t sysClk);
void delay_ms(uint32_t n);
void delay_us(uint32_t n);
void delay_xms(uint32_t n);


#endif
