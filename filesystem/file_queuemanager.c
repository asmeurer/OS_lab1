/* The File System Queue Manager!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include "file_queuemanager.h"
#include "definitions.h"

/* This is the same as our queuemanager from the process manager, except the
 * queue elements are now queue_message_t (see ipc_definitions.h).  */

/* Boilerplate initialization (since we can't use malloc) */

fcb error_file =
{
    .filename = "\0",
    .bits = 0,
    .dirHead = null,
    .blockhead = null,
    .blocktail = null,
    .next = null,
    .prev = null,
    .device_num = -1            /* This is where any error codes will
                                 * go. */
};

/**
 * Function to reset the deinit flag for the given queue.
 */
void deinit(struct dir_queue_t *queue) {
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

    return(ERROR_SUCCESS);
}


/**
 * Dequeues a message structure from a queue.
 * @param message_queue_enum The enum for the queue (The enum matches with the integer value).
 * @return Returns the message block that was dequeued. The source field of the message returns error codes.
 */
fcb *dir_dequeue(struct dir_queue_t *queue){

    if (queue->initialized == 0) {
        error_file.device_num = ERROR_BAD_DIR_QUEUE;
        return error_file;
    }

    /*If queue is empty*/
    if (queue->head == null) {
        error_file.device_num = ERROR_DIR_QUEUE_EMPTY;
        return error_file;
    }

    fcb ret = *queue->head;
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
    return(ret);
}

fcb *delete(struct dir_queue_t *queue, fcb *to_delete)
{

    if (queue->head == null && queue->tail == null) {
        /* The queue is empty */
        error_file.device_num = ERROR_DIR_QUEUE_EMPTY;
        return error_file;
    }

    /*If process doesn't exist*/
    if (to_delete == null){
        error_file.device_num = ERROR_BAD_FILE_PTR;
        return error_file;
    }
    fcb ret = *to_delete;
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
    return (ret);
}
