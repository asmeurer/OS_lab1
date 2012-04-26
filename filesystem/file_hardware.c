#include "file_hardware.h"
#include "file_queuemanager.h"

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

struct block_queue_t *malloc_block_queue()
{
    return malloc(sizeof(struct block_queue_t));
}

struct dir_queue_t *malloc_dir_queue()
{
    return malloc(sizeof(struct dir_queue_t));
}
