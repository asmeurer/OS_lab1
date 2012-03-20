/* The IPC Queue manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef IPC_QUEUE_MANAGER_H
#define IPC_QUEUE_MANAGER_H

#include "ipc_definitions.h"
void init(enum SCHEDS current_scheduler);
struct queue_message_t *get_process(enum MESSAGE_QUEUES message_queue_enum);
struct message *find_nonempty(struct queue_message_t *queue);
int enqueue(enum MESSAGE_QUEUES message_queue_enum, int pid, int psw, int page_table, int *regs, int priority, int quantum_count, int group);
void clear(struct message *process);
struct message *find_process(enum MESSAGE_QUEUES message_queue_enum, int pid);
struct message dequeue(enum MESSAGE_QUEUES message_queue_enum);
struct message delete(enum MESSAGE_QUEUES message_queue_enum, struct message *temp);

#endif
