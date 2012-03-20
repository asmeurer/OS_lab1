/* The IPC!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/
#include "ipc.h"

int 


int send(enum message_queue_enum source_queue, enum message_queue_enum dest_queue, char* message_string){
    
    
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

