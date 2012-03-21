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

#define null 0
#define MAX_MESSAGES 5
#define MESSAGE_SIZE 256

/*Global Error Codes*/
#define ERROR_SUCCESS 0
#define ERROR_QUEUE_EMPTY -1
#define ERROR_QUEUE_FULL -2
#define ERROR_SOURCE_QUEUE_NOT_EXIST -3
#define ERROR_DEST_QUEUE_NOT_EXIST -4
#define ERROR_MAX_STRING_LENGTH -5

/* This makes it easier to reference the message queues in external
 * functions.  Since the numbering of enum elements coincides exactly with
 * their integer names, you can interchange the enum name and the integer
 * value at any time.  */

enum MESSAGE_QUEUES {
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE
} message_queue_enum;

/* This is similar to the process manager queue manager.  This is the struct
 * for the message itself (the queue elements). */

struct message {
    enum MESSAGE_QUEUES source;
    enum MESSAGE_QUEUES destination;
    char string[MESSAGE_SIZE];
    struct message *next;
    struct message *prev;
    int empty;
};

/* This is the struct for the message queue. */

struct queue_message_t {
    struct message *head;
    struct message *tail;
    int size;
    struct message *top;
    int initialized;
};

#endif
