/* The IPC!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

int move(enum QUEUES from_queue, enum QUEUES to_queue){
    struct process_control_block temp = dequeue(from_queue);
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
