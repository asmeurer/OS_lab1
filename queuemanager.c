/* The Queue manager!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include "queuemanager.h"

struct process_control_block _new[1];
struct process_control_block _waiting[MAX_PROCESSES];
struct process_control_block _ready[MAX_PROCESSES];
struct process_control_block _terminated[MAX_PROCESSES];
struct process_control_block _running[1];

struct queue_t new =
{.head = null,
 .tail = null,
 .size = 1,
 .top = _new
};

struct queue_t waiting =
{.head = null,
 .tail = null,
 .size = MAX_PROCESSES,
 .top = _waiting
};

struct queue_t ready =
{.head = null,
 .tail = null,
 .size = MAX_PROCESSES,
 .top = _ready
};

struct queue_t terminated =
{.head = null,
 .tail = null,
 .size = MAX_PROCESSES,
 .top = _terminated
};

struct queue_t running =
{.head = null,
 .tail = null,
 .size = 1,
 .top = _running
};

struct process_control_block error_process =
{.pid = -1,
 .psw = 0,
 .page_table = 0,
 .regs = {0, 0, 0},
 .next = null,
 .prev = null,
 .empty = 0
};

void init() {
    int i = 0;
    counter = 0;

    new.head = null;
    new.tail = null;
    for (i = 0; i < new.size; i++) {
	clear(&new.top[i]);
    }
    waiting.head = null;
    waiting.tail = null;
    for (i = 0; i < waiting.size; i++) {
	clear(&waiting.top[i]);
    }
    ready.head = null;
    ready.tail = null;
    for (i = 0; i < ready.size; i++) {
	clear(&ready.top[i]);
    }
    terminated.head = null;
    terminated.tail = null;
    for (i = 0; i < terminated.size; i++) {
	clear(&terminated.top[i]);
    }
    running.head = null;
    running.tail = null;
    for (i = 0; i < running.size; i++) {
	clear(&running.top[i]);
    }
}

struct queue_t *get_process(enum QUEUES queue_enum) {
    switch (queue_enum) {
    case NEW:
        return &new;
    case WAITING:
	return &waiting;
    case READY:
	return &ready;
    case TERMINATED:
	return &terminated;
    case RUNNING:
	return &running;
    default:
        /* This will never be reached, but it silences a warning from the
         * compiler. */
        return &new;
    }
}

struct process_control_block *find_nonempty(struct queue_t *queue) {
    int i = 0;
    for (i = 0; i < queue->size; i++) {
        if (queue->top[i].empty == 1) {
            return &queue->top[i];
        }
    }
    return null;
}

int enqueue(struct queue_t *queue, int pid, int psw, int page_table, int *regs) {
    /* Enqueue */
    struct process_control_block *newprocess = find_nonempty(queue);
    int i = 0;

    if (!newprocess) {
        /* The queue is full */
        return(-1);
    }

    newprocess->pid = pid;
    newprocess->psw = psw;
    newprocess->page_table = page_table;
    for (i = 0; i < NUM_REGS; i++) {
        newprocess->regs[i] = regs[i];
    }

    if (queue->tail) {
        /* The queue already has elements */
        newprocess->next = queue->tail;
        queue->tail->prev = newprocess;
    } else {
        /* This is the first element of the queue */
        queue->head = newprocess;
    }
    queue->tail = newprocess;
    newprocess->empty = 0;

    return(0);
}

/*Process must exist for clear function*/
void clear(struct process_control_block *process){
    int i = 0;
    process->pid = 0;
    process->psw = 0;
    process->page_table = 0;
    for (i = 0; i < NUM_REGS; i++) {
        process->regs[i] = 0;
    }
    process->next = null;
    process->prev = null;
    /*Set as empty*/
    process->empty = 1;
}


struct process_control_block dequeue(struct queue_t *queue){
    /*If queue is empty*/
    if (queue->head == null){
        error_process.pid = -1;
        return error_process;
    }

    struct process_control_block ret = *queue->head;
    struct process_control_block *temp = queue->head;
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

struct process_control_block *find_process(struct queue_t *queue, int id){
    struct process_control_block *temp = queue->tail;
    while(temp != null){
        if(id == temp->pid){
            return temp;
        }
        temp = temp->next;
    }
    return null;
}

struct process_control_block delete(struct queue_t *queue, int id){
    struct process_control_block *temp = find_process(queue, id);

    if (queue->head == null && queue->tail == null) {
        /* The queue is empty */
        error_process.pid = -2;
        return error_process;
    }

    /*If process doesn't exist*/
    if (temp == null){
        error_process.pid = -1;
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
