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

/* struct message error_message  = */
/* {.source = -1, */
/*  .destination = -1, */
/*  .string = "", */
/*  .next = null, */
/*  .prev = null, */
/*  .empty = 0 */
/* }; */

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
void *dequeue(struct queue_t queue){

    struct queue_message_t *queue = get_message(message_queue_enum);


    if (queue->initialized == 0){
        error_message.source = ERROR_DEST_QUEUE_NOT_EXIST;
        return error_message;
    }

    /*If queue is empty*/
    if (queue->head == null){
        error_message.source = ERROR_QUEUE_EMPTY;
        return error_message;
    }

    struct message ret = *queue->head;
    struct message *temp = queue->head;
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
    clear(temp);
    return(ret);
}

void *delete(struct queue_t queue, void *to_delete)
{
    struct queue_t *queue = get_process(queue_enum);

    if (queue->head == null && queue->tail == null) {
        /* The queue is empty */
        error_process.pid = ERROR_QUEUE_EMPTY;
        return error_process;
    }

    /*If process doesn't exist*/
    if (temp == null){
        error_process.pid = ERROR_PROCESS_NOT_EXIST;
        return error_process;
    }
    struct process_control_block ret = *temp;
    /*If entry is only one in queue*/
    if(temp->next == null && temp->prev == null){
        queue->head = null;
        queue->tail = null;
    }
    /*If entry is at tail*/
    else if(temp->prev == null){
        queue->tail = temp->next;
        queue->tail->prev = null;
    }
    /*If entry is at head*/
    else if(temp->next == null){
        queue->head = temp->prev;
        queue->head->next = null;
    }
    /*If entry is in the middle*/
    else{
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }
    clear(temp);
    return (ret);
}
