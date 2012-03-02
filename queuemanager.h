/* The Queue manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include "definitions.h"
void init(enum SCHEDS current_scheduler);
struct queue_t *get_process(enum QUEUES queue_enum);
struct process_control_block *find_nonempty(struct queue_t *queue);
int enqueue(enum QUEUES queue_enum, int pid, int psw, int page_table, int *regs, int priority, int quantum_count, int group);
void clear(struct process_control_block *process);
struct process_control_block *find_process(enum QUEUES queue_enum, int pid);
struct process_control_block dequeue(enum QUEUES queue_enum);
struct process_control_block delete(enum QUEUES queue_enum, struct process_control_block *temp);

#endif
