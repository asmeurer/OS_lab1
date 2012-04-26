#include "file_hardware.h"
#include "file_queuemanager.h"

/* This is the simulation of the hardware.  Normally this would happen in the
 * device itself.  The data structures used here are "stored" on the device
 * itself, so we simulate that by calling malloc() and free().  We assume that
 * the data structures returned by these functions (i.e., memory pointers)
 * would be equivalent to the data structures returned by an actual
 * device.

 * The way it works is the filesystem manager calls these functions whenever it
 * creates or deletes a file or block.  Then this gives it the new memory for
 * that file or block, which is "one the device".  The filesystem manager
 * then writes to these data structures normally, which is the equivalent of
 * "writing to the device". */

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
