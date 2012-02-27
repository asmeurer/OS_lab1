/* The Queue manager!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include "queuemanager.h"

struct process_control_block _new[1];
struct process_control_block _waiting[MAX_PROCESSES];
struct process_control_block _ready0[MAX_PROCESSES];
struct process_control_block _ready1[MAX_PROCESSES];
struct process_control_block _ready2[MAX_PROCESSES];
struct process_control_block _ready3[MAX_PROCESSES];
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

struct queue_t ready0 =
{.head = null,
 .tail = null,
 .size = MAX_PROCESSES,
 .top = _ready0
};

struct queue_t ready1 =
{.head = null,
 .tail = null,
 .size = MAX_PROCESSES,
 .top = _ready1
};

struct queue_t ready2 =
{.head = null,
 .tail = null,
 .size = MAX_PROCESSES,
 .top = _ready2
};

struct queue_t ready3 =
{.head = null,
 .tail = null,
 .size = MAX_PROCESSES,
 .top = _ready3
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

void init(enum SCHEDS current_scheduler) {
    int i = 0;
    process_counter = 0;
    pid_counter = 0;
    global_quantum_count = 0;
    current_group = READY0;
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
    ready0.head = null;
    ready0.tail = null;
    for (i = 0; i < ready0.size; i++) {
        clear(&ready0.top[i]);
    }

    if(scheduler == 0){ 
    	ready1.head = null;
  	  ready1.tail = null;
    	for (i = 0; i < ready1.size; i++) {
		clear(&ready1.top[i]);
    	}
    	ready2.head = null;
    	ready2.tail = null;
    	for (i = 0; i < ready2.size; i++) {
        	clear(&ready2.top[i]);
   	 }
    	ready3.head = null;
    	ready3.tail = null;
    	for (i = 0; i < ready3.size; i++) {
        	clear(&ready3.top[i]);
	    }
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
    case READY0:
        return &ready0;
    case READY1:
        return &ready1;
    case READY2:
        return &ready2;
    case READY3:
        return &ready3;
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

int enqueue(enum QUEUES queue_enum, int pid, int psw, int page_table, int *regs, int priority, int quantum_count, int group) {
    /* Enqueue */
    struct queue_t *queue = get_process(queue_enum);
    struct process_control_block *newprocess = find_nonempty(queue);
    int i = 0;

    if (!newprocess) {
        /* The queue is full */
        return(ERROR_QUEUE_FULL);
    }

    newprocess->pid = pid;
    newprocess->psw = psw;
    newprocess->page_table = page_table;
    for (i = 0; i < NUM_REGS; i++) {
        newprocess->regs[i] = regs[i];
    }
    newprocess->priority = priority;
    newprocess->quantum_count = quantum_count;
	newprocess->group = group;

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

    return(ERROR_SUCCESS);
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
    process->priority = 0;
    process->quantum_count = 0;
}


struct process_control_block dequeue(enum QUEUES queue_enum){

    struct queue_t *queue = get_process(queue_enum);
    /*If queue is empty*/
    if (queue->head == null){
        error_process.pid = ERROR_QUEUE_EMPTY;
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

struct process_control_block *find_process(enum QUEUES queue_enum, int id){
    struct queue_t *queue = get_process(queue_enum);

    struct process_control_block *temp = queue->tail;
    while(temp != null){
        if(id == temp->pid){
            return temp;
        }
        temp = temp->next;
    }
    return null;
}

/**
 * @param queue The queue that it is deleating from
 * @param temp The process control block that is being deleted
 */
struct process_control_block delete(enum QUEUES queue_enum, struct process_control_block *temp){
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
