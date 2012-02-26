/* The Process Manager!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include"processmanager.h"
#include"queuemanager.h"

void age_process(){
    struct queue_t *queue_temp = get_process(READY0);
    struct process_control_block *temp = queue_temp->head;
    while (temp != null){
        temp->quantum_count++;
        if(temp->quantum_count >= temp->priority){
            temp->quantum_count = 0;
            if(temp->priority < 20){
                temp->priority++;
            }
        }
        temp = temp->next;
    }
}

int set_group(int group){

	struct queue_t *temp = get_process(NEW); 
	/* case new is full */
	if(temp->head != null){
		return -2;
	}	
	/* for the Group Scheduler, associates the group arg to which 
 * 	group to place the process in */

	if(scheduler == 0){
		if(group == 0){
			temp->head->Group = READY0; 
			return move(NEW, READY0);
		}
		else if(group == 1){
			temp->head->Group = READY1;
			return move(NEW, READY1);
		}
		else if(group == 2){
			temp->head->Group = READY2;
			return move(NEW, READY2);
		}
		else if(group == 3){
			temp->head->Group = READY3;
			return move(NEW, READY3);

		}else{
			return -4;
		}		

	/* For the Priority scheduler, just moves the process to the default group
 * 	READY0		*/

	}else {
		return move(NEW, READY0);	
	}
}


int go(){
    struct queue_t *temp = get_process(RUNNING);
    /*Running queue full, process already running*/
    if (temp->head != null){
        return -2;
    }
    return move(READY0, RUNNING);
}

int eoquantum(){
    age_process();
    struct queue_t *temp = get_process(RUNNING);
    
    /*TODO: Differenciate between different schedulers to do below code*/
    if(temp->head != null){
        if(temp->head->priority > 1){
            temp->head->priority--;
        }
    }
    /*END TODO*/
    return move(RUNNING, READY0);
}

int eolife(){
    return move(RUNNING, TERMINATED);
}

/* wait() is a system call, so the name conflicts in the test runner */
int wait_(){
    struct queue_t *temp = get_process(RUNNING);
    /*TODO: Differenciate between different schedulers to do below code*/
    if(temp->head != null){
        if(temp->head->priority < 20){
            temp->head->priority++;
        }
    }
    /*END TODO*/
    return move(RUNNING, WAITING);
}

int move(enum QUEUES from_queue, enum QUEUES to_queue){
    struct process_control_block temp = dequeue(get_process(from_queue));
    /*Nothing in queue, recoverable*/
    if(temp.pid == -1){
        return -1;
    }
    int error = enqueue(get_process(to_queue), temp.pid, temp.psw, temp.page_table, temp.regs, temp.priority, temp.quantum_count);
    /*Queue full, unrecoverable error*/
    if(error == -1){
        return -666;
    }
    return 0;
}

int unwait(int pid){
    struct process_control_block temp = delete(get_process(WAITING), pid);
    /*Pid doesn't exist*/
    if(temp.pid == -1){
        return -1;
    }
    /*Nothing in queue, recoverable*/
    else if(temp.pid == -2){
        return -2;
    }
    
    int error = enqueue(get_process(READY0), temp.pid, temp.psw, temp.page_table, temp.regs, temp.priority, temp.quantum_count);
    /*Queue full, unrecoverable error*/
    if(error == -1){
        return -666;
    }
    return 0;
}

int create(int psw, int page_table, int *reg){
    int error;
    /*If max allowed processes are reached*/
    if(process_counter >= MAX_PROCESSES){
        return -2;
    }
    /*TODO: Create a unique pid in new queue instead of here*/
    int pid = pid_counter;
    pid_counter++;
    /*Find if process already exists*/
    if ((find_process(get_process(WAITING), pid) != null) ||
        ((find_process(get_process(READY0), pid)) != null) ||
        ((find_process(get_process(READY1), pid)) != null) ||
        ((find_process(get_process(READY2), pid)) != null) ||
        ((find_process(get_process(READY3), pid)) != null) ||
        ((find_process(get_process(TERMINATED), pid)) != null) ||
        ((find_process(get_process(RUNNING), pid)) != null)) {
        return -3;
    }

    /*Creates process with default priority of 10*/
    /*TODO: Differenciate between schedualers for default priority of 10 or 0*/
    error = enqueue(get_process(NEW), pid, psw, page_table, reg, 10, 0);

    /*If new queue is full*/
    if (error == -1){
        return -666;
    }
    process_counter++;
    /*-1 for nothing in queue (fatal), -666 for fatal error*/
    return move(NEW, READY0);
}

int empty_term(){
    struct queue_t *terminated = get_process(TERMINATED);
    int i = 0;
    /*If terminated queue is empty, return -1*/
    if (terminated->head == null){
        return -1;
    }
    terminated->head = null;
    terminated->tail = null;
    for (i = 0; i < terminated->size; i++) {
        if(terminated->top[i].empty == 0){
            process_counter--;
        }
        clear(&terminated->top[i]);
    }
    return 0;
}
