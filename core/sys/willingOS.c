#include <stdint.h>
#include "willingOS.h"
#include "coreDef.h"
#include "sysTick.h"
#include "task.h"

void initNVICPriorityGroup(uint32_t priorityGroup);
void initKernel( void );
	
void initWilling(OSErr *err) {
    *err = OSErrNone;
     initSysTick(72);

    /*
        初始化中断优先级group，f103只用了4位优先级Bits[7:4]
    */
     initNVICPriorityGroup(NVIC_PriorityGroup_4); 

     initKernel();
}

void initNVICPriorityGroup(uint32_t priorityGroup) {
    SysCtrlBlock->AIRCR = AIRCR_VECTKEY_MASK | priorityGroup;
}


void willingStart(OSErr *err) {
    initKernel();
    OSStart();
}









