/* The Queue manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

/* NULL is in stdio.h.  Redefining NULL gives a warning, so call it null
   instead. */
   

#ifndef QUEUEMANAGER_H
#define QUEUEMANAGER_H

#define null 0
#define NUM_REGS 3
#define MAX_PROCESSES 20

struct process_control_block {
    int pid;   /* Process ID */
    int psw;   /* Program status word */
    int page_table;   /* Pagetable info */
    int regs[NUM_REGS];   /* Array of registers */
    struct process_control_block *next;
    struct process_control_block *prev;
    int empty;
};

struct queue_t {
    struct process_control_block *head;
    struct process_control_block *tail;
    int size;
    struct process_control_block *top;
};

enum QUEUES {
    NEW,
    WAITING,
    READY,
    TERMINATED,
    RUNNING
} queue_enum;

void init();
struct queue_t *get_process(enum QUEUES queue_enum);
struct process_control_block *find_nonempty(struct queue_t *queue);
int enqueue(struct queue_t *queue, int pid, int psw, int page_table, int *regs);
void clear(struct process_control_block *process);
struct process_control_block *find_process(struct queue_t *queue, int pid);
struct process_control_block dequeue(struct queue_t *queue);
struct process_control_block delete(struct queue_t *queue, int id);

/*Process manager function declerations*/
int go();
int eoquantum();
int eolife();
int wait();
int move(enum QUEUES from_queue, enum QUEUES to_queue);
int unwait(int pid);
int create(int pid, int psw, int page_table, int reg0, int reg1, int reg2);

#endif
