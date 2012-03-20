/* The process manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include"ipc_definitions.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>

void printmessage(struct message MESSAGE);
char *fgetstring(FILE* fFile);
