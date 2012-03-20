/* The process manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef IPC_TEST_H
#define IPC_TEST_H
#include"ipc_definitions.h"
#include<stdio.h>

void printmessage(struct message MESSAGE);
char *fgetstring(FILE* fFile);
void list_MQ(enum MESSAGE_QUEUES queuelist);
void printmessage(struct message MESSAGE);

#endif
