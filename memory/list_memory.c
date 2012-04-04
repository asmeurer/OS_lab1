/* Functions to list out the memory */

#include <stdio.h>
#include "memory_definitions.h"

void list_backing_store() {
    const int backing_store_free_size = BACK_STORE_NUM_FRAME/8;

    printf("%d\n", backing_store_free_size);

}
