/* The File system Queue manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef FILE_QUEUE_MANAGER_H
#define FILE_QUEUE_MANAGER_H

#include "definitions.h"
#include "file_hardware.h"


/* This is the struct for the queues. */

struct dir_queue_t {
    fcb *head;
    fcb *tail;
    int size;
    int initialized;
};

void dir_deinit(struct dir_queue_t *queue);
void dir_init_queue(struct dir_queue_t *queue);
int dir_enqueue(struct dir_queue_t *queue, fcb *file);
fcb *dir_dequeue(struct dir_queue_t *queue);
fcb *dir_delete(struct dir_queue_t *queue, fcb *to_delete);

#endif
