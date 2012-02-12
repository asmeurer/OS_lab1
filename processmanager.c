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

/* enum QUEUES {
    NEW,
    WAITING,
    READY,
    TERMINATED,
    RUNNING
} queue_enum; */

/* NULL is in stdio.h.  Redefining NULL gives a warning, so call it null
   instead. */
#define null 0

int move(enum QUEUES from_queue, enum QUEUES to_queue){
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

int unwait(int pid){
	struct process_control_block temp = delete(get_process(WAITING), pid);
	/*Nothing in queue, recoverable*/
	if(temp.pid == -1){
		return -1;
	}
	/*Pid doesn't exist*/
	else if(temp.pid == -2){
		return -2;
	}
	int error = enqueue(get_process(READY), temp.pid, temp.psw, temp.page_table, temp.regs);
	/*Queue full, unrecoverable error*/
	if(error == -1){
		return -666;
	}
	return 0;

int create(int pid, int psw, int page_table, int reg0, int reg1, int reg2){

	int counter=0;
	int regs[3];

   if ((find_process(get_process(WAITING), pid)) != null){
		return -1; /*process already exsists */
		if((find_process(get_process(READY), pid)) != null){
			return -1; /*process already exsists */
			if((find_process(get_process(TERMINATED), pid)) != null){
				return -1; /*process already exsists */
			}
		}	
   }

   regs[0] = reg0;
   regs[1] = reg1;
   regs[2] = reg2;
   
   enqueue(NEW, pid, psw, page_table, regs); 
   counter++;
     
}
