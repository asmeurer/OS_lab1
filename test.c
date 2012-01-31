/* The test file! */

#include<stdio.h>
#include "queuemanager.c"

void printprocess(struct process_control_block process);
void listQ();

void printprocess(struct process_control_block process) {
  int i = 0;

  printf("\n");
  printf("pid: %d\n", process.pid);
  printf("psw: %d\n", process.psw);
  printf("page_table: %d\n", process.page_table);
  printf("regs:\n");
  for (i = 0; i < NUM_REGS; i++) {
    printf("  %d\n", process.regs[i]);
  }
  printf("\n");
}

void listQ() {
  struct process_control_block *temp = head;
  printf("Start of queue.\n");
  while (temp) {
    printprocess(*temp);
    temp = temp->prev;
  }
  printf("End of queue.\n\n");
}

int main() {
  int i = 0;
  int r = 0;

  init();
  listQ();
  int regs1[3] = {3, 4, 5};
  r = enqueue(0, 2, 3, regs1);
  printf("return value: %d\n", r);
  listQ();
  int regs2[3] = {10, 14, 23};
  r = enqueue(1, 21, 22, regs2);
  printf("return value: %d\n", r);
  listQ();
  for (i = 2; i < 21; i++){
    r = enqueue(i, i, i, regs1);
    printf("return value: %d\n", r);
    /* listQ(); */
  }
  listQ();
  return(0);
}
