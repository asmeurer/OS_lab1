/* The IPC header file

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef IPC_H
#define IPC_H

#include "ipc_definitions.h"
#include "ipc_queuemanager.h"


int send(enum MESSAGE_QUEUES source_queue, enum MESSAGE_QUEUES dest_queue, char* message_string);
int retrieve(enum MESSAGE_QUEUES dest_queue, char* buffer);

#endif
