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


/* struct queue_t {
    struct process_control_block *head;
    struct process_control_block *tail;
    int size;
    struct process_control_block *top;
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


int go_(){
	struct queue_t *temp = get_process_(RUNNING);
	/*Running queue full, process already running*/
	if (temp->head != null){
		return -2;
	}
	return move_(READY, RUNNING);
}

int eoquantum(){
	return move_(RUNNING, READY);
}

int eolife(){
	return move_(RUNNING, TERMINATED);
}

int wait_(){
	return move_(RUNNING, WAITING);
}

int move_(enum QUEUES from_queue, enum QUEUES to_queue){
	struct process_control_block temp = dequeue(get_process_(from_queue));
	/*Nothing in queue, recoverable*/
	if(temp.pid == -1){
		return -1;
	}
	int error = enqueue(get_process_(to_queue), temp.pid, temp.psw, temp.page_table, temp.regs);
	/*Queue full, unrecoverable error*/
	if(error == -1){
		return -666;
	}
	return 0;
}

int unwait_(int pid){
	struct process_control_block temp = delete(get_process_(WAITING), pid);
	/*Nothing in queue, recoverable*/
	if(temp.pid == -1){
		return -1;
	}
	/*Pid doesn't exist*/
	else if(temp.pid == -2){
		return -2;
	}
	int error = enqueue(get_process_(READY), temp.pid, temp.psw, temp.page_table, temp.regs);
	/*Queue full, unrecoverable error*/
	if(error == -1){
		return -666;
	}
	return 0;
}

int create_(int pid, int psw, int page_table, int *reg){
	int error;
	/*If max allowed processes are reached*/
	if(counter >= MAX_PROCESSES){
		return -2;
	}
	/*Find if process already exists*/
	if ((find_process(get_process_(WAITING), pid)) != null){
		return -3;
		if((find_process(get_process_(READY), pid)) != null){
			return -3;
			if((find_process(get_process_(TERMINATED), pid)) != null){
				return -3;
				if((find_process(get_process_(RUNNING), pid)) != null){
					return -3;
				}
			}
		}
	}
	error = enqueue(NEW, pid, psw, page_table, reg);
	/*If new queue is full*/
	if (error == -1){
		return -666;
	}
	counter++;
	/*-1 for nothing in queue (fatal), -666 for fatal error*/
	return move_(NEW, READY);
}
