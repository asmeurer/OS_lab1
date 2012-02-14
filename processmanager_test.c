/* The test file for the process manager!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include "processmanager_test.h"
#include "processmanager.h"
#include "queuemanager.h"

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
    struct queue_t *structqueue = get_process(queue);
	struct process_control_block *temp = structqueue->head;
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
	struct queue_t *temp = get_process(READY);
	printf("Next scheduled process PID: %d\n\n", temp->head->pid);
}

int main() {
    int i = 0;
	int error = 0;
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

    FILE *file = fopen("processmanager_tests", "r");

    while(fgets(line, 100, file) != NULL) {
   	/*printf("***reading the file***\n");*/
   	command = strtok(line, delim);

   	/* printf("%s\n", line); */
   	/* printf("%s\n", command); */

   	if (!strcmp(command, "INIT")) {
            printf("\n***INIT command issued***\n");
            init();
   	}
   	else if (!strcmp(command, "LIST")) {
        arg = strtok(NULL, delim);
		printf("\n***LISTING command issued***\n");
		if (!strcmp(arg, "NEW")){
			list_Q(NEW);
		}
		else if (!strcmp(arg, "WAITING")){
			list_Q(WAITING);
		}
		else if (!strcmp(arg, "READY")){
			list_Q(READY);
		}
		else if (!strcmp(arg, "TERMINATED")){
			list_Q(TERMINATED);
		}
		else if (!strcmp(arg, "RUNNING")){
			list_Q(RUNNING);
		}
		else if (!strcmp(arg, "ALL")){
			list_all();
		}
		else if (!strcmp(arg, "SCHED")){
			list_sched();
		}
		else{
			printf("Argument not recognized");
		}
   	}
   	else if (!strcmp(command, "GO")){
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
		error = unwait(pid);
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
			printf("Could not EOQUANTUM: No running processes\n");
		}
		else if (error == -666){
			printf("FATAL ERROR: SYSTEM EXIT\n");
			exit(-1);
		}
   	}
   	else if (!strcmp(command, "EOLIFE")) {
		printf("\n***EOLIFE command issued***\n");
		error = eolife();
		if (error == -1){
			printf("Could not EOLIFE: No running processes\n");
		}
		else if (error == -666){
			printf("FATAL ERROR: SYSTEM EXIT\n");
			exit(-1);
		}
   	}
	else if (!strcmp(command, "WAIT")) {
		printf("\n***WAIT command issued***\n");
		error = wait_();
		if (error == -1){
			printf("Could not WAIT: No running processes\n");
		}
		else if (error == -666){
			printf("FATAL ERROR: SYSTEM EXIT\n");
			exit(-1);
		}
   	}
	else if (!strcmp(command, "CREATE")) {
		printf("\n***CREATE command issued***\n");
		for (i = 0; i < 6; i++) {
		args[i] = strtok(NULL, delim);
		}
		pid = atoi(args[0]);
		psw = atoi(args[1]);
		page_table = atoi(args[2]);
		reg1 = atoi(args[3]);
		reg2 = atoi(args[4]);
		reg3 = atoi(args[5]);

		/* printf("pid: %d, psw: %d, page_table: %d, reg1: %d, reg2: %d, reg3: %d\n", pid, psw, page_table, reg1, reg2, reg3); */
		regs[0] = reg1;
		regs[1] = reg2;
		regs[2] = reg3;

		error = create(pid, psw, page_table, regs);
		if (error == -1 || error == -666) {
			printf("FATAL ERROR: SYSTEM EXIT\n");
			exit(-1);
		}
		else if (error == -2){
			printf("Could not CREATE: Maximum allowed processes reached\n");
		}
		else if (error == -3){
			printf("Could not CREATE: PID not unique\n");
		}
   	}
   	else if (!strcmp(command, "#")) {
		printf("\n%s\n", line);
	}
   	else {
            printf("Unrecognized command: %s\n", command);
   	}
    }

    fclose(file);

    return(0);
}
