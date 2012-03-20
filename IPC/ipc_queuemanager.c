/* The IPC Queue manager!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include"ipc_queuemanager.h"
#include"ipc_definitions.h"

struct message _0[MAX_MESSAGES];
struct message _1[MAX_MESSAGES];
struct message _2[MAX_MESSAGES];
struct message _3[MAX_MESSAGES];
struct message _4[MAX_MESSAGES];
struct message _5[MAX_MESSAGES];
struct message _6[MAX_MESSAGES];
struct message _7[MAX_MESSAGES];
struct message _8[MAX_MESSAGES];
struct message _9[MAX_MESSAGES];

struct queue_message_t zero =
{.head = null,
 .tail = null,
 .size = MAX_MESSAGES,
 .top = _0
};

struct queue_message_t one =
{.head = null,
 .tail = null,
 .size = MAX_MESSAGES,
 .top = _1
};

struct queue_message_t two =
{.head = null,
 .tail = null,
 .size = MAX_MESSAGES,
 .top = _2
};

struct queue_message_t three =
{.head = null,
 .tail = null,
 .size = MAX_MESSAGES,
 .top = _3
};

struct queue_message_t four =
{.head = null,
 .tail = null,
 .size = MAX_MESSAGES,
 .top = _4
};

struct queue_message_t five =
{.head = null,
 .tail = null,
 .size = MAX_MESSAGES,
 .top = _5
};

struct queue_message_t six =
{.head = null,
 .tail = null,
 .size = MAX_MESSAGES,
 .top = _6
};

struct queue_message_t seven =
{.head = null,
 .tail = null,
 .size = MAX_MESSAGES,
 .top = _7
};

struct queue_message_t eight =
{.head = null,
 .tail = null,
 .size = MAX_MESSAGES,
 .top = _8
};

struct queue_message_t nine =
{.head = null,
 .tail = null,
 .size = MAX_MESSAGES,
 .top = _9
};

struct message error_message  =
{.source = -1,
 .destination = -1,
 .string = "",
 .next = null,
 .prev = null,
 .empty = 0
};

void init(enum SCHEDS current_scheduler) {
    int i = 0;

    zero.head = null;
    zero.tail = null;
    for (i = 0; i < zero.size; i++) {
        clear(&zero.top[i]);
    }

    one.head = null;
    one.tail = null;
    for (i = 0; i < one.size; i++) {
        clear(&one.top[i]);
    }

    two.head = null;
    two.tail = null;
    for (i = 0; i < two.size; i++) {
        clear(&two.top[i]);
    }

    three.head = null;
    three.tail = null;
    for (i = 0; i < three.size; i++) {
        clear(&three.top[i]);
    }

    four.head = null;
    four.tail = null;
    for (i = 0; i < four.size; i++) {
        clear(&four.top[i]);
    }

    five.head = null;
    five.tail = null;
    for (i = 0; i < five.size; i++) {
        clear(&five.top[i]);
    }

    six.head = null;
    six.tail = null;
    for (i = 0; i < six.size; i++) {
        clear(&six.top[i]);
    }

    seven.head = null;
    seven.tail = null;
    for (i = 0; i < seven.size; i++) {
        clear(&seven.top[i]);
    }

    eight.head = null;
    eight.tail = null;
    for (i = 0; i < eight.size; i++) {
        clear(&eight.top[i]);
    }

    nine.head = null;
    nine.tail = null;
    for (i = 0; i < nine.size; i++) {
        clear(&nine.top[i]);
    }

}

struct queue_message_t *get_process(enum MESSAGE_QUEUES message_queue_enum) {
    switch (queue_enum) {
    case ONE:
        return &one;
    case TWO:
        return &two;
    case THREE:
        return &three;
    case FOUR:
        return &four;
    case FIVE:
        return &five;
    case SIX:
        return &six;
    case SEVEN:
        return &seven;
    case EIGHT:
        return &eight;
    case NINE:
        return &nine;
    default:
        /* This will never be reached, but it silences a warning from the
         * compiler. */
        return &zero;
    }
}

struct message *find_nonempty(struct queue_message_t *queue) {
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
