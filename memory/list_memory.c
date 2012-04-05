/* Functions to list out the memory */

#include <stdio.h>
#include "memory_definitions.h"
#include "list_memory.h"
#include "../shared/textcolor.c"

void list_backing_store() {
    int i;
    char str[sizeof(byte)*8 + 1];
    const int backing_store_free_size = BACK_STORE_NUM_FRAME/8;
    const int num_cols = 128;   /* Should be a multiple of 8.  128 will
                                       * give a perfect square. */
    const int row_division = 5; /* The number of rows to include before a row
                                 * divider. */
    print_row_separator(row_division, num_cols);
    printf("\n");
    printf("|     |");
    for (i = 0; i < num_cols; i++) {
        if (i && !((i + 1) % 8)) {
            printf("%-8d|", i - 7);
        }
    }

    for (i = 0; i < backing_store_free_size; i++) {
        if (!(i % (num_cols/8))) {
            if (!(i % row_division * num_cols/8)) {
                print_row_separator(row_division, num_cols);
            }
            printf("\n");
            printf("|%5d|", i*8);
        }
        itodots(backing_store_free[i], str);
        printf("%s|", str);
    }
    print_row_separator(row_division, num_cols);
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

void print_row_separator(int row_division, int num_cols) {
    int j;
    int k;
    printf("\n");
    printf("+");

    for (j = 0; j < 5; j++) {
        printf("-");
    }
    for (k = 0; k < num_cols/8; k++) {
        printf("+");
        for (j = 0; j < 8; j++) {
            printf("-");
        }
    }
    printf("+");
}
