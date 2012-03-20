/* The IPC!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/
#include "ipc.h"

int message_len(char* message_string){
	int i;
	int overflow = 1;
	
	for(i = 0; i < MESSAGE_SIZE; i++){
		if (message_string[i] == '\0'){
			overflow = 0;
			break;
		}
	}
	if (overflow){
		return ERROR_MAX_STRING_LENGTH;
	}
	else{
		return i;
	}
}


int send(enum MESSAGE_QUEUES dest_queue, int source, char* message_string){
    int error;
    if (message_len(message_string) == ERROR_MAX_STRING_LENGTH){
		return ERROR_MAX_STRING_LENGTH;
	}
	error = enqueue(dest_queue, source, int destination, char *string);
	
	
	
    struct message temp = dequeue(from_queue);
    /*Nothing in queue, recoverable*/
    if(temp.pid == ERROR_QUEUE_EMPTY){
        return ERROR_QUEUE_EMPTY;
    }
    int error = enqueue(to_queue, temp.pid, temp.psw, temp.page_table, temp.regs, temp.priority, temp.quantum_count, temp.group);
    /*Queue full, recoverable error*/
    if(error == ERROR_QUEUE_FULL){
        return ERROR_QUEUE_FULL;
    }
    return ERROR_SUCCESS;
}

int retrieve(enum message_queue_enum dest_queue, char* buffer){
	
}

