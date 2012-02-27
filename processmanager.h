/* The process manager header file.

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "definitions.h"
int go();
int eoquantum();
int eolife();
int wait_();
int move(enum QUEUES from_queue, enum QUEUES to_queue);
int unwait(int pid);
int create(int psw, int page_table, int *reg, int group);
int empty_term();
void age_process();
struct process_control_block *iterate();
int set_group(int group);
int switch_group();

#endif
