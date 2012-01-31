/* The Queue manager! */

#define NUM_REGS 3
#define MAX_PROCESSES 20
struct process_control_block {
  int pid;   /* Process ID */
  int psw;   /* Program status word */
  int page_table;   /* Pagetable info */
  int regs[NUM_REGS];   /* Array of registers */
  process_control_block *next;
  process_control_block *prev;
}

struct process_control_block process[MAX_PROCESSES];

int size = 0;

int enqueue(int pid, int psw, int page_table, int *regs);

int enqueue(int pid, int psw, int page_table, int *regs){
  if (size == MAX_PROCESSES) {
    return(-1)
      } else {
    /* Enqueue */

  }
}
