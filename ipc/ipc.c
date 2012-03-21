/* The IPC!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include "ipc.h"

/**
 * Function to find the message length.
 * @param message_string The string to find the length.
 * @return The length of the string, an error otherwise (i.e., it does not end
 * in the null terminator).
 */
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

/**
 * Function to send a message to the destination queue.
 * @param source_queue The enum for the message source (The enum matches with
 * the integer value).
 * @param dest_queue The enum for the message destination (The enum matches
 * with the integer value).
 * @param message_string The string message to be sent.
 * @return Returns an error code according to ipc_definitions.h.
 */
int send(enum MESSAGE_QUEUES source_queue, enum MESSAGE_QUEUES dest_queue, char* message_string){
    if (source_queue == dest_queue) {
        return ERROR_SEND_TO_SELF;
    }

    if (message_len(message_string) == ERROR_MAX_STRING_LENGTH){
        return ERROR_MAX_STRING_LENGTH;
    }

    /*Returns queue full, source/destination queue not exist*/
    return enqueue(source_queue, dest_queue, message_string);
}


/**
 * Function to retrieve a message from a queue.
 * @param dest_queue The enum for the queue to pull the message from (The enum
 * matches with the integer value).
 * @param buffer The buffer that the string will be read to.
 * @return Returns an error code according to ipc_definitions.h.
 */
int retrieve(enum MESSAGE_QUEUES dest_queue, char* buffer){
    struct message temp = dequeue(dest_queue);
    if (temp.source == ERROR_QUEUE_EMPTY){
        return ERROR_QUEUE_EMPTY;
    }
    else if(temp.source == ERROR_DEST_QUEUE_NOT_EXIST){
        return ERROR_DEST_QUEUE_NOT_EXIST;
    }
    else{
		int i = 0;
		for (i = 0; i < MESSAGE_SIZE; i++) {
			buffer[i] = temp.string[i];
			if (buffer[i] == '\0') {
				break;
			}
		}
        return ERROR_SUCCESS;
    }

}
