/* The definitions header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

/* NULL is in stdio.h.  Redefining NULL gives a warning, so call it null
   instead. */

#ifndef DEF_H
#define DEF_H

#define MAX_QUANTUM 5
#define null 0
#define NUM_REGS 3
#define MAX_PROCESSES 20

enum QUEUES {
    NEW,
    WAITING,
    READY0,
    READY1,
    READY2,
    READY3,
    TERMINATED,
    RUNNING
} queue_enum;

enum SCHEDS {
    GROUP,
    PRIORITY
} scheduler_enum;

struct process_control_block {
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
int scheduler; /* 0 for Group-Scheduler and 1 for Priority-Scheduler */
int pid_counter;
int global_quantum_count;

#endif
