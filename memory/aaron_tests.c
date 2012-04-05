#include <stdio.h>
#include "../shared/binary.c"
#include "memory_manager.c"
#include "list_memory.c"

void list_memory_test();
void free_empty_test();

int main() {
    list_memory_test();

    return 0;
}

void list_memory_test() {
    int i;

    for (i = 0; i < BACK_STORE_SIZE/8; i++) {
        backing_store_free[i] = 0;
    }

    set_back_addr_full(0);
    set_back_addr_full(129);
    set_back_addr_full(400);

    list_backing_store();
}


void free_empty_test(){
    char str[sizeof(int)*8 + 1];

    int b = 0b10000000;

    int i;
    for (i = 0; i < BACK_STORE_SIZE/8; i++) {
        backing_store_free[i] = ~0;
    }

    i = find_empty_back_addr();
    itobin(i, str);
    printf("free memory: %d (%s)\n", i, str);
    printf("error: %d\n", ERROR_NO_FREE_MEMORY);

    backing_store_free[1] = (byte)0b11011111;

    itobin(~backing_store_free[1], str);
    printf("~backing_store_free[1]\n");
    printf("\t%s (%d)\n", str, ~backing_store_free[1]);

    itobin(~backing_store_free[1] & b, str);
    printf("~backing_store_free[1] & b\n");
    printf("\t%s (%d)\n", str, ~backing_store_free[1] & b);

    itobin((b >> 1), str);
    printf("(b >> 1)\n");
    printf("\t%s\n", str);

    itobin((~backing_store_free[1]) & (b >> 1), str);
    printf("(~backing_store_free[1]) & (b >> 1)\n");
    printf("\t%s (%d)\n", str, (~backing_store_free[1]) & (b >> 1));

    itobin((~backing_store_free[1]) & (b >> 2), str);
    printf("(~backing_store_free[1]) & (b >> 2)\n");
    printf("\t%s (%d)\n", str, (~backing_store_free[1]) & (b >> 2));

    int addr = 10;
    int prefix = addr >> 3;
    int suffix = addr & 0x7;
    int suffix_bitmask = 1 << (7 - suffix);

    printf("prefix %d\n", prefix);
    printf("suffix %d\n", suffix);
    itobin(suffix_bitmask, str);
    printf("suffix_bitmask %s\n", str);


    i = find_empty_back_addr();
    itobin(i, str);
    printf("free memory: %d (%s)\n", i, str);

    itobin(backing_store_free[1], str);
    printf("backing_store_free[1] %s\n", str);

    i = set_back_addr_empty(10);
    itobin(backing_store_free[1], str);
    printf("backing_store_free[1] %s\n", str);
    printf("%d\n", i);

    i = set_back_addr_full(10);
    itobin(backing_store_free[1], str);
    printf("backing_store_free[1] %s\n", str);
    printf("%d\n", i);

    i = set_back_addr_full(10);
    itobin(backing_store_free[1], str);
    printf("backing_store_free[1] %s\n", str);
    printf("%d\n", i);


    i = set_back_addr_empty(10);
    itobin(backing_store_free[1], str);
    printf("backing_store_free[1] %s\n", str);
    printf("%d\n", i);


}
