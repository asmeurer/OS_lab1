#include "file_hardware.h"

void Free(void *ptr)
{
    free(ptr);
}

fcb *malloc_file()
{
    return malloc(sizeof(fcb));
}

block *malloc_block()
{
    return malloc(sizeof(block));
}
