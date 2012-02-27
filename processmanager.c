/* The Process Manager (with schedulers)!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include"processmanager.h"
#include"queuemanager.h"

/*Remove me*/
#include <stdio.h>

/**
 * A function that implements aging as well as finding the process with the highest priority to schedual next
 * NOTE: If there is nothing in the ready queue, highest_priority pointer will be null
 */
struct process_control_block *iterate(int do_aging){
    struct queue_t *queue_temp = get_process(READY0);
    struct process_control_block *temp = queue_temp->tail;
    struct process_control_block *highest_priority = temp;
    while (temp != null){
        if(do_aging == 0){
            /*Aging*/
            temp->quantum_count++;
            if(temp->quantum_count >= temp->priority){
                temp->quantum_count = 0;
                if(temp->priority < 20){
                    temp->priority++;
                }
            }
        }
        /*Find highest priority*/
        /*Since starting at head, if equal priority, don't want to replace highest_priority*/
        if(temp->priority >= highest_priority->priority){
            highest_priority = temp;
        }
        temp = temp->next;
    }
    return highest_priority;
}

int set_group(int group){
	/* for the Group Scheduler, associates the group arg to which
     *     group to place the process in */
    if (scheduler == GROUP){
        if(group == 0){
			return move(NEW, READY0);
        }
        else if(group == 1){
            return move(NEW, READY1);
        }
        else if(group == 2){
            return move(NEW, READY2);
        }
        else if(group == 3){
            return move(NEW, READY3);

        } else {
            return -4;  /* Invalid group number */
        }

        /* For the Priority scheduler, just moves the process to the default group
         *     READY0        */

    } else if (scheduler == PRIORITY) {
        return move(NEW, READY0);
    }
    return 0;
}

int switch_group(){
    global_quantum_count = 0;
    switch(current_group){
    case READY0:
        current_group = READY1;
    case READY1:
        current_group = READY2;
    case READY2:
        current_group = READY3;
    case READY3:
        current_group = READY0;
    default:
        return -666;
    }
}


int go(){
    struct queue_t *running_queue = get_process(RUNNING);
    struct process_control_block *run_me_next;
    int error;
    int i = 0;
    /*Running queue full, process already running, do eoquantum*/
    if (running_queue->head != null){
        /*Group Fair Share*/
        if (scheduler == GROUP){
            error = move(RUNNING, running_queue->head->group);
            /*If empty queue error, unrecoverable because error checked above*/
            if (error == -666 || error == -1){
                return -666;
            }
            global_quantum_count++;
            if (global_quantum_count >= MAX_QUANTUM){
                error = switch_group();
                if (error == -666){
                    return -666;
                }
            }
        }
        /*Priority*/
        else if (scheduler == PRIORITY){
            run_me_next = iterate(1);
            /*If there is nothing in the ready queue when the process is running*/
            /*Set next scheduled process as the current running process*/
            if (run_me_next == null){
                run_me_next = running_queue->head;
            }
            /*Decrease the priority of running process because of eoquantum call*/
            if(running_queue->head->priority > 1){
                running_queue->head->priority--;
            }
            error = move(RUNNING, READY0);
            /*If empty queue error, unrecoverable because error checked above*/
            if (error == -666 || error == -1){
                return -666;
            }
        }
    }
    /*Nothing in running queue*/
    else{
        /*Group fair share*/
        if (scheduler == GROUP){
            global_quantum_count++;
            if (global_quantum_count >= MAX_QUANTUM){
                error = switch_group();
                if (error == -666){
                    return -666;
                }
            }
        }
        /*Priority*/
        else{
            run_me_next = iterate(1);
            /*No ready processes*/
            if (run_me_next->priority == null){
                return -1;
            }
        }
    }
    /*Schedual next process*/

	/*Group fair share*/
	if (scheduler == GROUP){
		i = 0;
		/*Runs through 4 times, checking all 4 groups*/
		while (i < 4){
			error = move(current_group, RUNNING);
			/*If empty ready queue in group*/
			/*Switch group*/
			if (error == -1){
				/*If all queues are empty*/
				if (i == 3){
					return -1;
				}
				switch_group();
			}
			/*Unrecoverable error*/
			else if (error == -666){
				return -666;
			}
			/*Success*/
			else if (error == 0){
				break;
			}
			i++;
		}
	}
	/*Priority*/
	else{
		struct process_control_block temp = delete(READY0, run_me_next);
		/*Pid doesn't exist, unrecoverable since iterate() should have found run_me_next*/
		if(temp.pid == -1){
			return -666;
		}
		/*Nothing in queue, unrecoverable, since already checked for empty queue*/
		else if(temp.pid == -2){
			return -666;
		}
		error = enqueue(RUNNING, temp.pid, temp.psw, temp.page_table, temp.regs, temp.priority, temp.quantum_count, temp.group);
		/*Queue is full, unrecoverable, since GO should have eoquantum*/
		if (error == -1){
			return -666;
		}
	}

    return 0;
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
    struct process_control_block temp = dequeue(from_queue);
    /*Nothing in queue, recoverable*/
    if(temp.pid == -1){
        return -1;
    }
    int error = enqueue(to_queue, temp.pid, temp.psw, temp.page_table, temp.regs, temp.priority, temp.quantum_count, temp.group);
    /*Queue full, unrecoverable error*/
    if(error == -1){
        return -666;
    }
    return 0;
}



int unwait(int pid){
    struct process_control_block temp = delete(WAITING, find_process(WAITING, pid));
    /*Pid doesn't exist*/
    if(temp.pid == -1){
        return -1;
    }
    /*Nothing in queue, recoverable*/
    else if(temp.pid == -2){
        return -2;
    }

    if(scheduler == GROUP){

        int error = enqueue(temp.group, temp.pid, temp.psw, temp.page_table, temp.regs, temp.priority, temp.quantum_count, temp.group);
        /*Queue full, unrecoverable error*/
        if(error == -1){
            return -666;
        }
    }else if(scheduler == PRIORITY){

        int error = enqueue(READY0, temp.pid, temp.psw, temp.page_table, temp.regs, temp.priority, temp.quantum_count, temp.group);
        /*Queue full, unrecoverable error*/
        if(error == -1){
            return -666;
        }
    }


    return 0;
}

int create(int psw, int page_table, int *reg, int group){
    int error;
    /*If max allowed processes are reached*/
    if(process_counter >= MAX_PROCESSES){
        return -2;
    }
    /*TODO: Create a unique pid in new queue instead of here*/
    int pid = pid_counter;
    pid_counter++;
    /*Find if process already exists*/
    if ((find_process(WAITING, pid) != null) ||
        ((find_process(READY0, pid)) != null) ||
        ((find_process(READY1, pid)) != null) ||
        ((find_process(READY2, pid)) != null) ||
        ((find_process(READY3, pid)) != null) ||
        ((find_process(TERMINATED, pid)) != null) ||
        ((find_process(RUNNING, pid)) != null)) {
        return -3;
    }

    /*Creates process with default priority of 10*/
    /*TODO: Differenciate between schedualers for default priority of 10 or 0*/
    error = enqueue(NEW, pid, psw, page_table, reg, 10, 0, group);

    /*If new queue is full*/
    if (error == -1){
        return -666;
    }
    process_counter++;
	
    /*-1 for nothing in queue (fatal), -666 for fatal error*/
    return set_group(group);
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
