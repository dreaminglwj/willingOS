#ifndef INCLUDE_WILLING_H
#define INCLUDE_WILLING_H
#include <stdlib.h>
#include "coreDef.h"

#define WILLING_OS_KERNEL_VERSION "V0.0.1"

#define wFALSE ( ( Base_t ) 0 )
#define wTRUE  ( ( Base_t ) 1 )

#define wPASS ( wTRUE )
#define wFAIL ( wFALSE )

/* 系统错误 */
#define errMEMORY_ALLOCATE_FAIL (-1)


#ifndef willingPOINTER_SIZE_TYPE
    #define willingPOINTER_SIZE_TYPE uint32_t
#endif

#endif
