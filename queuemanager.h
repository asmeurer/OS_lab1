/* The Queue manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

/* NULL is in stdio.h.  Redefining NULL gives a warning, so call it null
   instead. */


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

int counter;

void init_();
struct queue_t *get_process_(enum QUEUES queue_enum);
struct process_control_block *find_nonempty(struct queue_t *queue);
int enqueue(struct queue_t *queue, int pid, int psw, int page_table, int *regs);
void clear(struct process_control_block *process);
struct process_control_block *find_process(struct queue_t *queue, int pid);
struct process_control_block dequeue(struct queue_t *queue);
struct process_control_block delete(struct queue_t *queue, int id);

/*Process manager function declerations*/
int go_();
int eoquantum();
int eolife();
int wait_();
int move_(enum QUEUES from_queue, enum QUEUES to_queue);
int unwait_(int pid);
int create_(int pid, int psw, int page_table, int *reg);
