/* The IPC!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/
#include "ipc.h"

/*Array is an array of 10 integers of 0 for not initalized and 1 for initalized*/
void init(int array[]){
	int i;
	for (i = 0; i < 10; i++){
		init_queue(i, array[i]);
	}
}

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
	/*Returns queue full, source/destination queue not exist*/
	return enqueue(source_queue, dest_queue, message_string);
}

int retrieve(enum MESSAGE_QUEUES dest_queue, char* buffer){
	struct message temp = dequeue(dest_queue);
	if (temp.source == ERROR_QUEUE_EMPTY){
		return ERROR_QUEUE_EMPTY;
	}
	else if(temp.source == ERROR_DEST_QUEUE_NOT_EXIST){
		return ERROR_DEST_QUEUE_NOT_EXIST;
	}
	else{
		buffer = temp.string;
		return ERROR_SUCCESS;
	}
	
}

