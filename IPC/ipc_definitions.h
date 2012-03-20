/* The definitions header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

/* NULL is in stdio.h.  Redefining NULL gives a warning, so call it null
   instead. */

#ifndef IPC_DEF_H
#define IPC_DEF_H

#define MAX_QUANTUM 5
#define null 0
#define NUM_REGS 3
#define MAX_PROCESSES 20
#define MAX_PRIORITY 10
#define MAX_MESSAGES 5

/*Global Error Codes*/
#define ERROR_SUCCESS 0
#define ERROR_QUEUE_EMPTY -1
#define ERROR_QUEUE_FULL -2
#define ERROR_QUEUE_NOT_EXIST -3

enum MESSAGE_QUEUES {
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE
}
    message_queue_enum;


struct message
{
    int source;
    int destination;
    char string[256];
    struct message *next;
    struct message *prev;
    int empty;
}

    struct queue_message_t
    {
        struct message *head;
        struct message *tail;
        int size;
        struct message *top;
    }


#endif
