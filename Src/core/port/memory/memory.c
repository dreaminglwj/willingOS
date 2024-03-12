#include <stdlib.h>
#include "coreDef.h"


void *willingMalloc( size_t dstSize ) {
    void * rlt = NULL;
    rlt = malloc( dstSize );
	
	willingAssert(rlt);

    return rlt;
}

void willingFree( void *var ) {
    if (var) {
        free(var);
    }
}
