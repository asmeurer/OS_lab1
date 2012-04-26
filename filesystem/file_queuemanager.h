/* The File system Queue manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef FILE_QUEUE_MANAGER_H
#define FILE_QUEUE_MANAGER_H

#include "definitions.h"


extern fcb error_file;

extern block error_block;

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
int search_blocks(struct block_queue_t *queue, unsigned short addr);

void block_init_queue(struct block_queue_t *queue);
int block_enqueue(struct block_queue_t *queue, block *to_enqueue);
block *block_dequeue(struct block_queue_t *queue);
block *block_delete(struct block_queue_t *queue, block *to_delete);

#endif
