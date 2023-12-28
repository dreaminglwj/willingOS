#ifndef WILLING_OS_H
#define WILLING_OS_H

typedef  enum  os_err {
    OSErrNone = 0u 
} OSErr;

#endif

void initWilling(OSErr *err);
void willingStart(OSErr *err);
