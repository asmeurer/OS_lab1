/* Functions to list out the memory */

#include <stdio.h>
#include "memory_definitions.h"
#include "list_memory.h"

void list_backing_store() {
    int i;
    char str[sizeof(byte)*8 + 1];
    const int backing_store_free_size = BACK_STORE_NUM_FRAME/8;
    const int num_cols = 128 + 8*8;   /* Should be a multiple of 8.  128 will
                                       * give a perfect square. */

    for (i = 0; i < backing_store_free_size; i++) {
        if (!(i % (num_cols/8))) {
            if (i) {
                printf("\n");
            }
            printf("%5d ", i*8);
        }
        itodots(backing_store_free[i], str);
        printf("%s", str);
    }
    printf("\n");
}

void itodots(int n, char *str) {
    int place = 0;
    int i;
    const int bytes_in_byte = 8*sizeof(byte);

    for (i = 0; i < bytes_in_byte; i++){
        if (n & 1){
            str[bytes_in_byte - 1 - place] = '*';
        } else {
            str[bytes_in_byte - 1 - place] = '.';
        }
        n >>= 1;
        place++;
    }
    str[bytes_in_byte] = '\0';
}
