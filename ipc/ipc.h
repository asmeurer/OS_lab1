/* The IPC header file

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef IPC_H
#define IPC_H
#include "ipc_definitions.h"

int send(enum message_queue_enum source_queue, enum message_queue_enum dest_queue, char* message_string);
int retrieve(enum message_queue_enum dest_queue, char* buffer);

#endif
