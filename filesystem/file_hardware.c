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

block_queue_t *malloc_block_queue()
{
    return malloc(sizeof(block_queue_t));
}

dir_queue_t *malloc_dir_queue()
{
    return malloc(sizeof(dir_queue_t));
}
