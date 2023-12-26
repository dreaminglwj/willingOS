#include <stdlib.h>


void *willingMalloc( size_t dstSize ) {
    void * rlt = NULL;
    rlt = malloc( dstSize );

    return rlt;
}

void willingFree( void *var ) {
    if (var) {
        free(var);
    }
}