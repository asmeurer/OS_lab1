/* The File System Queue Manager!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include "file_queuemanager.h"
#include "definitions.h"

/* This is similar our queuemanager from the process manager and ipc, except the
 * queue elements are now fcb or blocks, and they take pointers (because the
 * hardware simulation can call malloc).  */

/**
 * Function to reset the deinit flag for the given queue.
 */
void dir_deinit(struct dir_queue_t *queue) {
    queue->initialized = 0;
}

/**
 * Function to clear and set the initilized flag for the specified queue.
 */
void dir_init_queue(struct dir_queue_t *queue) {
    fcb *temp;
    fcb *tempnext;

    temp = queue->tail;
    while (temp != null) {
        tempnext = temp->next;
        Free(temp);
        temp = tempnext;
    }

    queue->head = null;
    queue->tail = null;
    queue->size = 0;
    queue->initialized = 1;
}


/**
 * Enqueues a file into a queue.
 */
int dir_enqueue(struct dir_queue_t *queue, fcb *file) {
    /* Enqueue */

    if (queue->initialized == 0){
        return ERROR_SOURCE_QUEUE_NOT_EXIST;
    }

    if (file == null) {
        /* This should not happen */
        return(ERROR_BAD_FILE_PTR);
    }

    if (queue->tail) {
        /* The queue already has elements */
        file->next = queue->tail;
        queue->tail->prev = file;
    } else {
        /* This is the first element of the queue */
        queue->head = file;
    }
    queue->tail = file;

    queue->size++;
    return(ERROR_SUCCESS);
}


/**
 * Dequeues a message structure from a queue.
 * @param message_queue_enum The enum for the queue (The enum matches with the integer value).
 * @return Returns the message block that was dequeued. The source field of the message returns error codes.
 */
fcb *dir_dequeue(struct dir_queue_t *queue){

    if (queue->initialized == 0) {
        error_file.error = ERROR_BAD_DIR_QUEUE;
        return &error_file;
    }

    /*If queue is empty*/
    if (queue->head == null) {
        error_file.error = ERROR_DIR_QUEUE_EMPTY;
        return &error_file;
    }

    fcb *ret = queue->head;
    /*If entry is only one in queue*/
    if(queue->head->prev == null){
        queue->head = null;
        queue->tail = null;
    }
    else{
        /*Reset head pointer*/
        queue->head = queue->head->prev;
        queue->head->next = null;
    }

    queue->size--;
    return(ret);
}

fcb *dir_delete(struct dir_queue_t *queue, fcb *to_delete)
{

    if (queue->head == null && queue->tail == null) {
        /* The queue is empty */
        error_file.error = ERROR_DIR_QUEUE_EMPTY;
        return &error_file;
    }

    /*If process doesn't exist*/
    if (to_delete == null){
        error_file.error = ERROR_BAD_FILE_PTR;
        return &error_file;
    }
    fcb *ret = to_delete;
    /*If entry is only one in queue*/
    if(to_delete->next == null && to_delete->prev == null){
        queue->head = null;
        queue->tail = null;
    }
    /*If entry is at tail*/
    else if(to_delete->prev == null){
        queue->tail = to_delete->next;
        queue->tail->prev = null;
    }
    /*If entry is at head*/
    else if(to_delete->next == null){
        queue->head = to_delete->prev;
        queue->head->next = null;
    }
    /*If entry is in the middle*/
    else{
        to_delete->prev->next = to_delete->next;
        to_delete->next->prev = to_delete->prev;
    }

    queue->size--;
    return (ret);
}

/**
 * Function to reset the deinit flag for the given queue.
 */
void block_deinit(struct block_queue_t *queue) {
    queue->initialized = 0;
}

/**
 * Function to search through the blocks of a fcb and checking their address
 */
int seach_blocks(struct block_queue_t *queue, unsigned short addr){
    block *temp;
    temp = queue->tail;

    while(temp != null){
        if(temp->addr == addr){
            return 1;
        }
        temp= temp->next;
    }
    return 0;
}
/**
 * Function to clear and set the initilized flag for the specified queue.
 */
void block_init_queue(struct block_queue_t *queue) {
    block *temp;
    block *tempnext;

    temp = queue->tail;
    while (temp != null) {
        tempnext = temp->next;
        Free(temp);
        temp = tempnext;
    }

    queue->head = null;
    queue->tail = null;
    queue->size = 0;
    queue->initialized = 1;
}


/**
 * Enqueues a file into a queue.
 */
int block_enqueue(struct block_queue_t *queue, block *to_enqueue) {
    /* Enqueue */

    if (queue->initialized == 0){
        return ERROR_SOURCE_QUEUE_NOT_EXIST;
    }

    if (to_enqueue == null) {
        /* This should not happen */
        return(ERROR_BAD_BLOCK_PTR);
    }

    if (queue->tail) {
        /* The queue already has elements */
        to_enqueue->next = queue->tail;
        queue->tail->prev = to_enqueue;
    } else {
        /* This is the first element of the queue */
        queue->head = to_enqueue;
    }
    queue->tail = to_enqueue;

    queue->size++;
    return(ERROR_SUCCESS);
}


/**
 * Dequeues a message structure from a queue.
 * @param message_queue_enum The enum for the queue (The enum matches with the integer value).
 * @return Returns the message block that was dequeued. The source field of the message returns error codes.
 */
block *block_dequeue(struct block_queue_t *queue){

    if (queue->initialized == 0) {
        error_block.error = ERROR_BAD_BLOCK_QUEUE;
        return &error_block;
    }

    /*If queue is empty*/
    if (queue->head == null) {
        error_block.error = ERROR_BLOCK_QUEUE_EMPTY;
        return &error_block;
    }

    block *ret = queue->head;
    /*If entry is only one in queue*/
    if(queue->head->prev == null){
        queue->head = null;
        queue->tail = null;
    }
    else{
        /*Reset head pointer*/
        queue->head = queue->head->prev;
        queue->head->next = null;
    }

    queue->size--;
    return(ret);
}

block *block_delete(struct block_queue_t *queue, block *to_delete)
{

    if (queue->head == null && queue->tail == null) {
        /* The queue is empty */
        error_block.error = ERROR_BLOCK_QUEUE_EMPTY;
        return &error_block;
    }

    /*If process doesn't exist*/
    if (to_delete == null){
        error_block.error = ERROR_BAD_BLOCK_PTR;
        return &error_block;
    }
    block *ret = to_delete;
    /*If entry is only one in queue*/
    if(to_delete->next == null && to_delete->prev == null){
        queue->head = null;
        queue->tail = null;
    }
    /*If entry is at tail*/
    else if(to_delete->prev == null){
        queue->tail = to_delete->next;
        queue->tail->prev = null;
    }
    /*If entry is at head*/
    else if(to_delete->next == null){
        queue->head = to_delete->prev;
        queue->head->next = null;
    }
    /*If entry is in the middle*/
    else{
        to_delete->prev->next = to_delete->next;
        to_delete->next->prev = to_delete->prev;
    }

    queue->size--;
    return (ret);
}
