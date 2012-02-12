/* The Process Manager!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include"queuemanager.h"


/* struct process_control_block {
    int pid;
    int psw;
    int page_table;
    int regs[NUM_REGS];
    struct process_control_block *next;
    struct process_control_block *prev;
    int empty;
}; */

/* NULL is in stdio.h.  Redefining NULL gives a warning, so call it null
   instead. */
#define null 0

int move(enum queue_enum from_queue, enum queue_enum to_queue){
	struct process_control_block temp = dequeue(get_process(from_queue));
	/*Nothing in queue, recoverable*/
	if(temp.pid == -1){
		return -1;
	}
	int error = enqueue(get_process(to_queue), temp.pid, temp.psw, temp.page_table, temp.regs);
	/*Queue full, unrecoverable error*/
	if(error == -1){
		return -666;
	}
	return 0;
}