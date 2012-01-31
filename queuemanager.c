/* The Queue manager! */

#define NULL 0
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

struct process_control_block process[MAX_PROCESSES];

int i = 0;
struct process_control_block *head = NULL;
struct process_control_block *tail = NULL;

void init();
struct process_control_block *find_nonempty();
int enqueue(int pid, int psw, int page_table, int *regs);

void init() {
  for (i = 0; i < MAX_PROCESSES; i++) {
  process[i].empty = 0;
 }
}

struct process_control_block *find_nonempty() {
  for (i = 0; i < MAX_PROCESSES; i++) {
    if (process[i].empty == 1) {
      return &process[i];
    }
  }
  return NULL;
}

int enqueue(int pid, int psw, int page_table, int *regs) {
  /* Enqueue */
  struct process_control_block *newprocess = find_nonempty();

  if (!newprocess) {
    /* The queue if full */
    return(-1);
  }

  newprocess->pid = pid;
  newprocess->psw = psw;
  newprocess->page_table = page_table;
  for (i = 0; i < NUM_REGS; i++) {
      newprocess->regs[i] = regs[i];
    }

  if (tail) {
    /* The queue already has elements */
    tail->prev = newprocess;
    newprocess->next = tail;
  } else {
    /* This is the first element of the queue */
    head = newprocess;
  }
  tail = newprocess;

  return(0);
}
