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

/*Global Error Codes*/
#define ERROR_SUCCESS 0
#define ERROR_QUEUE_EMPTY -1
#define ERROR_QUEUE_FULL -2
#define ERROR_QUEUE_NOT_EXIST -3


struct message_control_block {
    int pid;   /* Process ID */
    int psw;   /* Program status word */
    int page_table;   /* Pagetable info */
    int regs[NUM_REGS];   /* Array of registers */
    struct process_control_block *next;
    struct process_control_block *prev;
    int empty;
    int priority;
    int quantum_count;
    enum QUEUES group;
};

struct queue_t {
    struct process_control_block *head;
    struct process_control_block *tail;
    int size;
    struct process_control_block *top;
};

int process_counter;
int pid_counter;
int global_quantum_count;
enum QUEUES current_group;

#endif
