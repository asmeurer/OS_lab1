/* The File system Queue manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef FILE_QUEUE_MANAGER_H
#define FILE_QUEUE_MANAGER_H

#include "definitions.h"

fcb error_file =
{
    .filename = "\0",
    .bits = 0,
    .dirHead = null,
    .containing_dir = null,
    .block_queue = null,
    .next = null,
    .prev = null,
    .device_num = -1,
    .error = -1                   /* This is where any error codes will go. */
};

block error_block =
{
    .addr = 0,
    .next = null,
    .prev = null,
    .error = -1                 /* This is where any error codes will go. */
};

/* This is the struct for the queues. */

struct dir_queue_t {
    fcb *head;
    fcb *tail;
    int size;
    int initialized;
};

struct block_queue_t
{
    block *head;
    block *tail;
    int size;
    int initialized;
};

void dir_init_queue(struct dir_queue_t *queue);
int dir_enqueue(struct dir_queue_t *queue, fcb *file);
fcb *dir_dequeue(struct dir_queue_t *queue);
fcb *dir_delete(struct dir_queue_t *queue, fcb *to_delete);
int seach_blocks(struct block_queue_t *queue, unsigned short addr);

void block_init_queue(struct block_queue_t *queue);
int block_enqueue(struct block_queue_t *queue, block *to_enqueue);
block *block_dequeue(struct block_queue_t *queue);
block *block_delete(struct block_queue_t *queue, block *to_delete);

#endif
