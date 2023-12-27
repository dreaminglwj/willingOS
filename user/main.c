#include "sysTick.h"
#include "willingOS.h"

int main() {
   OSErr err;
   initWilling(&err);

   // 创建任务

   willingStart(&err);

    return 0;
}