/* The File System Queue Manager!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include "file_queuemanager.h"
#include "definitions.h"
#include "file_hardware.h"

/* This is similar our queuemanager from the process manager and ipc, except the
 * queue elements are now fcb or blocks, and they take pointers (because the
 * hardware simulation can call malloc).  */

fcb error_file =
{
    .filename = "\0",
    .bits = 0,
    .dirHead = null,
    .parent_dir = null,
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

/**
 * Function to clear the specified directory queue.
 *
 * @param queue The directory queue to initialize.
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
 * Enqueues a file into a directory queue.
 *
 * @param queue The directory queue to enqueue from.
 *
 * @param file The file to enqueue to the queue.
 *
 * @return Returns an error code on error, else ERROR_SUCCESS.
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
 * Dequeues a file from a directory queue.  Useful for recursively deleting
 * all files from a directory.
 *
 * @param queue The directory queue to dequeue from.
 *
 * @return Returns a pointer to the fcb that was dequeued. The error field of
 * the fcb contains any error codes.
 */
fcb *dir_dequeue(struct dir_queue_t *queue){

    if (queue->initialized == 0) {
        error_file.error = ERROR_BAD_DIR_QUEUE;
        return &error_file;
    }

    /* If queue is empty */
    if (queue->head == null) {
        error_file.error = ERROR_DIR_QUEUE_EMPTY;
        return &error_file;
    }

    fcb *ret = queue->head;
    /* If entry is only one in queue */
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

/**
 * Deletes a file from a directory queue.
 *
 * @param queue The directory queue to delete from.
 *
 * @param to_delete The file to delete.
 *
 * @return Returns a pointer to the deleted fcb.  Any error codes will be in
 * the error field.
 */
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
 * Function to search through the blocks of a fcb and check their addresses.
 *
 * @param queue The block queue to search through.
 *
 * @param addr The address to search for.
 *
 * @return Returns 1 if a block in the queue has the given address and 0
 * otherwise.
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
 * Function to clear the specified block queue.
 *
 * @param queue The block queue to initialize.
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
 * Enqueues a block into a block queue.
 *
 * @param queue The block queue to enqueue into
 *
 * @param to_enqueue The block to enqueue
 *
 * @return Returns an error code on error, else ERROR_SUCCESS.
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
 * Dequeues a block from a block queue.  Useful when deleting a file.
 *
 * @param queue The block queue to dequeue from.
 *
 * @return Returns the block that was dequeued. The error field of the block
 * contains any error codes.
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

/**
 * Deletes a block from a block queue.
 *
 * @param queue The block queue to delete from.
 *
 * @param to_delete The block to delete.
 *
 * @return Returns a pointer to the deleted block.  Any error codes will be in
 * the error field.
 */
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
