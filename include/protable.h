#ifndef PORTBALE_H
#define PORTBALE_H

#include "coreDef.h"

#if BYTE_ALIGNMENT == 8
    #define BYTE_ALIGNMENT_MASK ( 0x0007 )
#endif

Stack_t *initStack( Stack_t *topOfStack, TaskFunc_t pxCode, void *pvParameters );

#endif 