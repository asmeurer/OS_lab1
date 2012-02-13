/* The process manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef PROCESS_MANAGER_TEST_H
#define PROCESS_MANAGER_TEST_H

#include "definitions.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char* enum_to_string(enum QUEUES queue);
void printprocess(struct process_control_block process);
void list_Q(enum QUEUES queue);
void list_all();
void list_sched();

#endif
