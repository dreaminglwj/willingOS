#include <stdint.h>
#include "willingOS.h"
#include "coreDef.h"
#include "sysTick.h"
#include "task.h"

void initNVICPriorityGroup(uint32_t priorityGroup);
	
void initWilling(OSErr *err) {
    *err = OSErrNone;
     initSysTick(72);

     initNVICPriorityGroup(NVIC_PriorityGroup_4);
}


void willingStart(OSErr *err) {
    OSStart();
}

void initNVICPriorityGroup(uint32_t priorityGroup) {
    SysCtrlBlock->AIRCR = AIRCR_VECTKEY_MASK | priorityGroup;
}
