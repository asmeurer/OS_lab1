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
int i = 0

int enqueue(int pid, int psw, int page_table, int *regs);

int enqueue(int pid, int psw, int page_table, int *regs){
  if (size == MAX_PROCESSES) {
    return(-1)
      } else {
    /* Enqueue */
    process_control_block[size].pid = pid;
    process_control_block[size].psw = psw;
    process_control_block[size].page_table = page_table;
    for (i = 0; i < NUM_REGS; i++){
      process_control_block[size].regs[i] = regs[i];
    }
  }
}
