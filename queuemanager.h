/* The Queue manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include "definitions.h"
void init();
struct queue_t *get_process(enum QUEUES queue_enum);
struct process_control_block *find_nonempty(struct queue_t *queue);
int enqueue(struct queue_t *queue, int pid, int psw, int page_table, int *regs, int priority, int quantum_count);
void clear(struct process_control_block *process);
struct process_control_block *find_process(struct queue_t *queue, int pid);
struct process_control_block dequeue(struct queue_t *queue);
struct process_control_block delete(struct queue_t *queue, struct process_control_block *temp);

#endif
