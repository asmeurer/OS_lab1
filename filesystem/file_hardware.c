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

#include "file_hardware.h"
#include "definitions.h"
#include "file_queuemanager.h"

/**
 * Hardware simulation of free.

 * This simulates a hardware call to free a data structure stored in the
 * filesystem.  In the simulated implementation, it actually calls the
 * system call free(), since we are simulating the filesystem hardware in
 * actual memory.
 *
 * @param ptr Pointer to the data structure to be freed.
 */
void Free(void *ptr)
{
    free(ptr);
}

/**
 * Hardware simulation of creating a new file control block.
 *
 * This simulates the hardware writing a file control block to the filesysten.
 * In the simulated implementation, it just calls the system call malloc(),
 * since the simulated filesystem hardware lives in actual memory.
 *
 * @return Returns a pointer to the allocated file control block.
 */
fcb *malloc_file()
{
    return malloc(sizeof(fcb));
}

/**
 * Hardware simulation of getting a new block from the filesystem.
 *
 * This simulates the hardware giving us a new free block from the filesystem.
 * We actually keep track of what is free or not in the filesystem manager
 * (this data structure would actually be stored on the filesystem itself).
 *
 * In the simulated implementation, it just calls the system call malloc(),
 * since the simulated filesystem hardware lives in actual memory.
 *
 * @return Returns a pointer to the block.
 */
block *malloc_block()
{
    return malloc(sizeof(block));
}

/**
 * Hardware simulation of creating a new block queue.
 *
 * This simulates the hardware writing a new block queue data structure in the
 * filesysten.  In an actual filesystem, this would just be part of the file
 * control block, but in our simulated implementation, it is easier to keep it
 * separate (but there's no reason why we couldn't have put it in there, as C
 * allows nested structs).
 *
 * In the simulated implementation, it just calls the system call
 * malloc(), since the simulated filesystem hardware lives in actual memory.
 *
 * @return Returns a pointer to the allocated block queue.
 */
struct block_queue_t *malloc_block_queue()
{
    struct block_queue_t *block_queue = malloc(sizeof(struct block_queue_t));
    block_queue->tail = null;
    return block_queue;
}

/**
 * Hardware simulation of creating a directory queue.
 *
 * This simulates the hardware writing a new directory queue data structure in
 * the filesysten.  In an actual filesystem, this would just be part of the
 * file control block, but in our simulated implementation, it is easier to
 * keep it separate (but there's no reason why we couldn't have put it in
 * there, as C allows nested structs, we just felt the implementation was
 * cleaner this way).
 *
 * In the simulated implementation, it just calls the system call
 * malloc(), since the simulated filesystem hardware lives in actual memory.
 *
 * @return Returns a pointer to the allocated directory queue.
 */
struct dir_queue_t *malloc_dir_queue()
{
    struct dir_queue_t *dir_queue =  malloc(sizeof(struct dir_queue_t));
    dir_queue->tail = null;
    return dir_queue;
}

/**
 * Flushes the buffer corresponding to buf_ptr.
 *
 * This is currently the only way to actually clear the buffer.  This
 * simulates the hardware finishing the operation for the buffer (read or
 * write).  It called either manually in the test interface (to simulate this
 * hardware event), or by certain filesystem manager functions, such as
 * unmount, that require the buffers to be empty to complete (in reality, they
 * would block on this event).
 */
int buf_flush(int buf_ptr)
{
    int i;
    if (buf_ptr < 0 || buf_ptr >= NUM_BUFFERS) {
        return ERROR_BUFFER_NOT_EXIST;
    }

    for (i = 0; i < BUFFER_SIZE; i++) {
        buffers[buf_ptr][i].addr = 0;
        buffers[buf_ptr][i].access_type = 0;
        buffers[buf_ptr][i].init = 0;
    }

    return ERROR_SUCCESS;
}
