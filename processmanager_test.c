/* The test file for the queuemanager!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "queuemanager.h"

/* Make the tests still work by just testing the ready queue */

/* enum QUEUES {
    NEW,
    WAITING,
    READY,
    TERMINATED,
    RUNNING
} queue_enum;
 */
 
char* enum_to_string(enum QUEUES queue);
void printprocess(struct process_control_block process);
void list_Q(enum QUEUES queue);
void list_all();
void list_sched();

void printprocess(struct process_control_block process) {
    int i = 0;

    printf("\n");
    printf("pid: %d\n", process.pid);
    printf("psw: %d\n", process.psw);
    printf("page_table: %d\n", process.page_table);
    printf("regs:\n");
    for (i = 0; i < NUM_REGS; i++) {
   	printf("  %d\n", process.regs[i]);
    }
    /* printf("\n"); */
}

char* enum_to_string(enum QUEUES queue){
    switch (queue) {
    case NEW:
        return "New Queue";
    case WAITING:
	return "Waiting Queue";
    case READY:
	return "Ready Queue";
    case TERMINATED:
	return "Terminated Queue";
    case RUNNING:
	return "Running Queue";
    default:
        return "No Such Queue";
    }
}

void list_Q(enum QUEUES queue) {
    struct queue_t structqueue = getprocess(queue);
	struct process_control_block *temp = structqueue.head;
    printf("Start of %s.\n", enum_to_string(queue));
    while (temp) {
   	printprocess(*temp);
   	temp = temp->prev;
    }
    printf("\nEnd of %s.\n\n", enum_to_string(queue));
}

void list_all(){
	list_Q(NEW);
	list_Q(WAITING);
	list_Q(READY);
	list_Q(TERMINATED);
	list_Q(RUNNING);
}

void list_sched(){
	struct queue_t *temp = getprocess(READY);
	printf("Next scheduled process PID: %d\n\n", temp->head.pid);
}

int main() {
    int i = 0;
    int r = 0;
	int error = 0;
    struct process_control_block ret;
    char line[100];
    char delim[] = " \n";
    char *command;
    char *args[6];
	char *arg;
    int pid = 0;
    int psw = 0;
    int page_table = 0;
    int reg1 = 0;
    int reg2 = 0;
    int reg3 = 0;
    int regs[3];

    init();

    FILE *file = fopen("queuemanager_tests", "r");

    while(fgets(line, 100, file) != NULL) {
   	/*printf("***reading the file***\n");*/
   	command = strtok(line, delim);

   	/* printf("%s\n", line); */
   	/* printf("%s\n", command); */

   	if (!strcmp(command, "INIT_Q")) {
            printf("\n***INIT command issued***\n");
            init();
   	}
   	else if (!strcmp(command, "LIST")) {
        arg = atoi(strtok(NULL, delim));
		printf("\n***LISTING command issued***\n");
		if (!strcmp(arg, "NEW"){
			list_Q(NEW);
		}
		else if (!strcmp(arg, "WAITING"){
			list_Q(WAITING);
		}
		else if (!strcmp(arg, "READY"){
			list_Q(READY);
		}
		else if (!strcmp(arg, "TERMINATED"){
			list_Q(TERMINATED);
		}
		else if (!strcmp(arg, "RUNNING"){
			list_Q(RUNNING);
		}
		else if (!strcmp(arg, "ALL"){
			list_all();
		}
		else if (!strcmp(arg, "SCHED"){
			list_sched();
		}
		else{
			printf("Argument not recognized");
		}
   	}
   	else if (!strcmp(command, "GO")) {
		printf("\n***GO command issued***\n");
		error = go();
		if (error == -1){
			printf("Could not GO: No ready processes\n");
		}
		else if (error == -2){
			printf("Could not GO: Process already running\n");
		}
		else if (error == -666){
			printf("FATAL ERROR: SYSTEM EXIT\n");
			exit(-1);
		}
   	}
   	else if (!strcmp(command, "UNWAIT")) {
		printf("\n***UNWAIT command issued***\n");
		pid = atoi(strtok(NULL, delim));
		error = unwait();
		if (error == -1){
			printf("Could not UNWAIT: No waiting processes\n");
		}
		else if (error == -2){
			printf("Could not UNWAIT: PID does not exist in waiting queue\n");
		}
		else if (error == -666){
			printf("FATAL ERROR: SYSTEM EXIT\n");
			exit(-1);
		}
   	}
   	else if (!strcmp(command, "EOQUANTUM")) {
		printf("\n***EOQUANTUM command issued***\n");
		error = eoquantum();
		if (error == -1){
			printf("Could not UNWAIT: No waiting processes\n");
		}
		else if (error == -2){
			printf("Could not UNWAIT: PID does not exist in waiting queue\n");
		}
		else if (error == -666){
			printf("FATAL ERROR: SYSTEM EXIT\n");
			exit(-1);
		}
   	}







   	else if (!strcmp(command, "")) {
            printf("\n***dequeueing***\n");
            ret = dequeue(&ready);
            if (ret.pid == -1) {
           	printf("Could not dequeue: queue empty.\n");
            } else {
           	printf("%d\n", ret.pid);
            }
   	}
   	else if (!strcmp(command, "delete")) {
            printf("\n***deleting***\n");
            pid = atoi(strtok(NULL, delim));
            ret = delete(&ready, pid);
            if (ret.pid == -1) {
           	printf("Could not dequeue: process not found.\n");
            } else if (ret.pid == -2) {
           	printf("Could not dequeue: queue empty.\n");
            } else {
           	printf("%d\n", ret.pid);
            }
   	}
   	else {
            printf("Unrecognized command: %s\n", command);
   	}
    }

    fclose(file);

    return(0);
}
