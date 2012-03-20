/* The IPC Queue manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef IPC_QUEUE_MANAGER_H
#define IPC_QUEUE_MANAGER_H

#include "ipc_definitions.h"

void init(enum MESSAGE_QUEUES message_queue_enum);
struct queue_message_t *get_message(enum MESSAGE_QUEUES message_queue_enum);
struct message *find_nonempty(struct queue_message_t *queue);
int enqueue(enum MESSAGE_QUEUES message_queue_enum, enum MESSAGE_QUEUES source, enum MESSAGE_QUEUES destination, char *string);
void clear(struct message *process);
struct message *find_message(enum MESSAGE_QUEUES message_queue_enum, int pid);
struct message dequeue(enum MESSAGE_QUEUES message_queue_enum);
struct message delete(enum MESSAGE_QUEUES message_queue_enum, struct message *temp);

#endif
