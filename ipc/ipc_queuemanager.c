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

void init(enum MESSAGE_QUEUES message_queue_enum) {
    int i = 0;

    switch (message_queue_enum) {
    case ZERO:
        zero.head = null;
        zero.tail = null;
        for (i = 0; i < zero.size; i++) {
            clear(&zero.top[i]);
        }
        break;
    case ONE:
            one.head = null;
        one.tail = null;
        for (i = 0; i < one.size; i++) {
            clear(&one.top[i]);
        }
        break;
    case TWO:
        two.head = null;
        two.tail = null;
        for (i = 0; i < two.size; i++) {
            clear(&two.top[i]);
        }
        break;
    case THREE:
        three.head = null;
        three.tail = null;
        for (i = 0; i < three.size; i++) {
            clear(&three.top[i]);
        }
        break;
    case FOUR:
        four.head = null;
        four.tail = null;
        for (i = 0; i < four.size; i++) {
            clear(&four.top[i]);
        }
        break;
    case FIVE:
        five.head = null;
        five.tail = null;
        for (i = 0; i < five.size; i++) {
            clear(&five.top[i]);
        }
        break;
    case SIX:
        six.head = null;
        six.tail = null;
        for (i = 0; i < six.size; i++) {
            clear(&six.top[i]);
        }
        break;
    case SEVEN:
        seven.head = null;
        seven.tail = null;
        for (i = 0; i < seven.size; i++) {
            clear(&seven.top[i]);
        }
        break;
    case EIGHT:
        eight.head = null;
        eight.tail = null;
        for (i = 0; i < eight.size; i++) {
            clear(&eight.top[i]);
        }
        break;
    case NINE:
        nine.head = null;
        nine.tail = null;
        for (i = 0; i < nine.size; i++) {
            clear(&nine.top[i]);
        }
        break;
    }
}

struct queue_message_t *get_message(enum MESSAGE_QUEUES message_queue_enum) {
    switch (message_queue_enum) {
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

int enqueue(enum MESSAGE_QUEUES message_queue_enum, int source, int destination, char *string) {
    /* Enqueue */
    struct queue_message_t *queue = get_message(message_queue_enum);
    struct message *newmessage = find_nonempty(queue);
    int i = 0;

    if (!newmessage) {
        /* The queue is full */
        return(ERROR_QUEUE_FULL);
    }

    newmessage->source = source;
    newmessage->destination = destination;
    for (i = 0; i < MESSAGE_SIZE; i++) {
        newmessage->string[i] = string[i];
        if (string[i] == '\0') {
            break;
        }
    }

    if (queue->tail) {
        /* The queue already has elements */
        newmessage->next = queue->tail;
        queue->tail->prev = newmessage;
    } else {
        /* This is the first element of the queue */
        queue->head = newmessage;
    }
    queue->tail = newmessage;
    newmessage->empty = 0;

    return(ERROR_SUCCESS);
}

/*Process must exist for clear function*/
void clear(struct message *m){
    int i = 0;
    m->source = -1;
    m->destination = -1;
    for (i = 0; i < MESSAGE_SIZE; i++) {
        m->string[i] = 0;
    }
    m->next = null;
    m->prev = null;
    /*Set as empty*/
    m->empty = 0;
}


struct message dequeue(enum MESSAGE_QUEUES message_queue_enum){

    struct queue_message_t *queue = get_message(message_queue_enum);
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
