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


int send(enum MESSAGE_QUEUES source_queue, enum MESSAGE_QUEUES dest_queue, char* message_string){
    if (message_len(message_string) == ERROR_MAX_STRING_LENGTH){
		return ERROR_MAX_STRING_LENGTH;
	}
	return enqueue(source_queue, dest_queue, message_string);
}

int retrieve(enum MESSAGE_QUEUES dest_queue, char* buffer){
	struct message temp = dequeue(dest_queue);
	if (temp.source == -1){
		return ERROR_QUEUE_EMPTY;
	}
	else{
		buffer = temp.string;
		return ERROR_SUCCESS;
	}
	
}
