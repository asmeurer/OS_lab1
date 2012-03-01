/* The test file for the process manager!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include "processmanager_test.h"
#include "processmanager.h"
#include "queuemanager.h"
#include "textcolor.c"

void printprocess(struct process_control_block process) {
    int i = 0;

    printf("pid: ");
    textcolor(BRIGHT, RED, BLACK);
    printf("%d\t", process.pid);
    textcolor(RESET, -1, -1);
    printf("psw: %d\t", process.psw);
    printf("p_table: %d\t", process.page_table);
    printf("regs:");
    for (i = 0; i < NUM_REGS; i++) {
        printf(" %d", process.regs[i]);
    }
    if (scheduler == GROUP){
        printf("\tgroup: ");
        textcolor(BRIGHT, MAGENTA, BLACK);
        printf("%s", enum_to_string(process.group));
        textcolor(RESET, -1, -1);
    }
    else{
        printf("\tpriority: ");
        textcolor(BRIGHT, CYAN, BLACK);
        printf("%d", process.priority);
        textcolor(RESET, -1, -1);
        printf("\tquantum_count: %d", process.quantum_count);
    }
    printf("\n");
}

char* enum_to_string(enum QUEUES queue){
    switch (queue) {
    case NEW:
        return "New Queue";
    case WAITING:
        return "Waiting Queue";
    case READY0:
        if(scheduler == GROUP){
            return "Group 0 Ready Queue";
        }
        else{
            return "Ready Queue";
        }
    case READY1:
        return "Group 1 Ready Queue";
    case READY2:
        return "Group 2 Ready Queue";
    case READY3:
        return "Group 3 Ready Queue";
    case TERMINATED:
        return "Terminated Queue";
    case RUNNING:
        return "Running Queue";
    default:
        return "No Such Queue";
    }
}

char* error_to_string(int error){
    switch (error) {
    case ERROR_QUEUE_EMPTY:
        return "Queue Empty";
    case ERROR_QUEUE_FULL:
        return "Queue Full";
    case ERROR_PROCESS_NOT_EXIST:
        return "Process Doesn't Exist";
    case ERROR_GROUP_NOT_EXIST:
        return "Group Doesn't Exist";
    case ERROR_SWITCH_DEFAULT:
        return "Switch Default Error";
    case ERROR_NO_READY_PROCESS:
        return "No Ready Processes";
    case ERROR_MAX_PROCESSES:
        return "Maximum Processes Reached";
    case ERROR_PROCESS_NOT_UNIQUE:
        return "Process Not Unique";
    default:
        return "No Such Error";
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
    printf("End of %s.\n\n", enum_to_string(queue));

}

void list_all(){
    list_Q(NEW);
    list_Q(WAITING);
    list_ready();
    list_Q(TERMINATED);
    list_Q(RUNNING);
}

void list_ready(){
    list_Q(READY0);
    if (scheduler == GROUP){
        list_Q(READY1);
        list_Q(READY2);
        list_Q(READY3);
    }
}

void list_sched(){
    struct queue_t *temp;
    struct process_control_block *pcb;
    if (scheduler == GROUP){
        temp = get_process(current_group);
        if (temp->head == null) {
            printf("Nothing is scheduled to run (ready queue empty)\n");
        } else {
            printprocess(*temp->head);
        }
    }
    else{
        pcb = iterate(0);
        if (pcb == null) {
            printf("Nothing is scheduled to run (ready queue empty)\n");
        } else {
            printprocess(*pcb);
        }
    }

}

/**
 *Gets a string from the file
 *@return The string that was in the file
 */
char *fgetstring(FILE* fFile){
    char c = 0;
    char *name = NULL;
    int counter = 1;

    /*Gets memory for the string*/
    name = (char*) malloc(sizeof(char));
    /*If memory exists*/
    if (name != NULL){
        /*Read characters one by one until '\n' (enter), ' ' (space), or feof (end of file) is found*/
        while(c != '\n' && !feof(fFile)){
            c = fgetc(fFile);
            name[counter-1] = c;
            counter++;
            name = (char*) realloc(name, sizeof(char) * counter);
            if (name == NULL){
                break;
            }
        }
        /*Enters terminator*/
        name[counter-2] = '\0';
    }

    return (name);
}
int main(int argc, char *argv[]) {
    int error = 0;
    char command[20];
    char args[10][10];
    int pid = 0;
    int psw = 0;
    int page_table = 0;
    int reg1 = 0;
    int reg2 = 0;
    int reg3 = 0;
    int group = 0;
    int regs[3];
    FILE *file;
	int priority = 0;
    if (argc == 1){
        file = stdin;
    }
    else if (argc == 2){
        file = fopen(argv[1], "r");
    }
    else{
        printf("Usage:\n./processmanager_test.o (to read from stdin)\n");
        printf("./processmanager_test.o <filename> (to read from filename)\n");
        exit(-1);
    }

    init(GROUP);


    while(1) {
        /*printf("***reading the file***\n");*/
        if (file == stdin) {
            printf("> ");
        }
        error = fscanf(file," %s", command);
        if (error == 1){
            /* printf("%s\n", line); */
            /* printf("%s\n", command); */

            if (!strcmp(command, "INIT")) {
                error = fscanf(file, " %s", args[0]);
                if (error == 1){
                    if (!strcmp(args[0], "GROUP")){
                        printf("\n***INIT command issued (%s)***\n", args[0]);
                        init(GROUP);
                    }
                    else if(!strcmp(args[0], "PRIORITY")){
                        printf("\n***INIT command issued (%s)***\n", args[0]);
                        init(PRIORITY);
                    }
                    else{
                        printf("Usage: INIT <GROUP | PRIORITY>\n");
                    }
                }
                else{
                    printf("Usage: INIT <GROUP | PRIORITY>\n");
                }

            }
            else if (!strcmp(command, "LIST")) {
                error = fscanf(file, " %s", args[0]);
                if (error == 1){
                    printf("\n***LISTING command issued (%s)***\n", args[0]);
                    if (!strcmp(args[0], "NEW")){
                        list_Q(NEW);
                    }
                    else if (!strcmp(args[0], "WAITING")){
                        list_Q(WAITING);
                    }
                    else if (!strcmp(args[0], "READY")){
                        list_ready();
                    }
                    else if (!strcmp(args[0], "TERMINATED")){
                        list_Q(TERMINATED);
                    }
                    else if (!strcmp(args[0], "RUNNING")){
                        list_Q(RUNNING);
                    }
                    else if (!strcmp(args[0], "ALL")){
                        list_all();
                    }
                    else if (!strcmp(args[0], "SCHED")){
                        list_sched();
                    }
                    else{
                        printf("Usage: LIST <NEW | WAITING | READY | TERMINATED | RUNNING | ALL | SCHED>\n");
                    }
                }
                else{
                    printf("Usage: LIST <NEW | WAITING | READY | TERMINATED | RUNNING | ALL | SCHED>\n");
                }
            }
            else if (!strcmp(command, "GO")){
                printf("\n***GO command issued***\n");
                error = go();
                if (error == ERROR_NO_READY_PROCESS){
                    printf("Could not GO: %s\n", error_to_string(error));
                }
                else if (error == ERROR_QUEUE_EMPTY ||
						error == ERROR_QUEUE_FULL ||
						error == ERROR_SWITCH_DEFAULT ||
						error == ERROR_PROCESS_NOT_EXIST){
                    printf("FATAL ERROR: %s\n", error_to_string(error));
                    exit(-1);
                }
            }
            else if (!strcmp(command, "UNWAIT")) {
                error = fscanf(file, " %d", &pid);
                if (error == 1){
                    printf("\n***UNWAIT command issued (PID: %d)***\n", pid);
                    error = unwait(pid);
                    if (error == ERROR_QUEUE_EMPTY){
                        printf("Could not UNWAIT: No waiting processes\n");
                    }
                    else if (error == ERROR_PROCESS_NOT_EXIST){
                        printf("Could not UNWAIT: PID does not exist in waiting queue\n");
                    }
                    else if (error == ERROR_QUEUE_FULL){
                        printf("FATAL ERROR: %s\n", error_to_string(error));
                        exit(-1);
                    }
                }
                else{
                    printf("Usage: UNWAIT <pid>\n");
                }
            }
			else if (!strcmp(command, "SET_PRIORITY")) {

				error = fscanf(file, " %d %d", &pid, &priority);
				if (error == 2){
					printf("\n***SET_PRIORITY command issued (PID: %d)***\n", pid);
					error = set_priority(pid, priority);
					if (error == ERROR_WRONG_SCHEDULER){
						printf("Could not SET_PRIORITY: Command only avaliable in Priority Scheduler\n");
					}
					else if (error == ERROR_PROCESS_NOT_EXIST){
						printf("Could not SET_PRIORITY: PID does not exist in ready queue\n");
					}
					else if (error == ERROR_INVALID_PARAMETER){
						printf("Could not SET_PRIORITY: Priority must be between 1 and %d\n", MAX_PRIORITY);
					}
				}
				else{
					printf("Usage: SET_PRIORITY <pid> <priority>\n");
				}

            }
            else if (!strcmp(command, "EOLIFE")) {
                printf("\n***EOLIFE command issued***\n");
                error = eolife();
                if (error == ERROR_QUEUE_EMPTY){
                    printf("Could not EOLIFE: No running processes\n");
                }
                else if (error == ERROR_QUEUE_FULL){
                    printf("FATAL ERROR: %s\n", error_to_string(error));
                    exit(-1);
                }
            }
            else if (!strcmp(command, "WAIT")) {
                printf("\n***WAIT command issued***\n");
                error = wait_();
                if (error == ERROR_QUEUE_EMPTY){
                    printf("Could not WAIT: No running processes\n");
                }
                else if (error == ERROR_QUEUE_FULL){
                    printf("FATAL ERROR: %s\n", error_to_string(error));
                    exit(-1);
                }
            }
            else if (!strcmp(command, "CREATE")) {
                /*Group Scheduler*/
                if (scheduler == GROUP){
                    error = fscanf(file, " %d %d %d %d %d %d", &psw, &page_table, &reg1, &reg2, &reg3, &group);
                    if (error == 6){
                        /* printf("pid: %d, psw: %d, page_table: %d, reg1: %d, reg2: %d, reg3: %d\n", pid, psw, page_table, reg1, reg2, reg3); */
                        regs[0] = reg1;
                        regs[1] = reg2;
                        regs[2] = reg3;

						error = create(psw, page_table, regs, group);
						if (error == ERROR_MAX_PROCESSES){
							printf("Could not CREATE: Maximum allowed processes reached\n");
						}
						else if (error == ERROR_GROUP_NOT_EXIST){
							printf("Could not CREATE: Invalid group number\n");
						}
						else if (error == ERROR_PROCESS_NOT_UNIQUE ||
						error == ERROR_QUEUE_FULL ||
						error == ERROR_QUEUE_EMPTY ||
						error == ERROR_QUEUE_FULL) {
							printf("FATAL ERROR: %s\n", error_to_string(error));
							exit(-1);
						}
						else if (error == ERROR_SUCCESS){
							printf("\n***CREATE command issued (PID: %d)***\n", (pid_counter - 1));
						}
					}
					else{
						printf("Usage: CREATE <psw> <page_table> <reg1> <reg2> <reg3> <group>\n");
					}
				}
				/*Priority Scheduler*/
				else{
					error = fscanf(file, " %d %d %d %d %d", &psw, &page_table, &reg1, &reg2, &reg3);
					if (error == 5){
						/* printf("pid: %d, psw: %d, page_table: %d, reg1: %d, reg2: %d, reg3: %d\n", pid, psw, page_table, reg1, reg2, reg3); */
						regs[0] = reg1;
						regs[1] = reg2;
						regs[2] = reg3;

						printf("\n***CREATE command issued (PID: %d)***\n", pid_counter);
						/*Priority scheduler defaults to group 0*/
						error = create(psw, page_table, regs, 0);
						if (error == ERROR_MAX_PROCESSES){
							printf("Could not CREATE: Maximum allowed processes reached\n");
						}
						else if (error == ERROR_PROCESS_NOT_UNIQUE ||
						error == ERROR_QUEUE_FULL ||
						error == ERROR_QUEUE_EMPTY ||
						error == ERROR_QUEUE_FULL ||
						error == ERROR_GROUP_NOT_EXIST) {
							printf("FATAL ERROR: %s\n", error_to_string(error));
							exit(-1);
						}
						else if (error == ERROR_SUCCESS){
							printf("\n***CREATE command issued (PID: %d)***\n", (pid_counter - 1));
						}
					}
					else{
						printf("Usage: CREATE <psw> <page_table> <reg1> <reg2> <reg3>\n");
					}
				}
            }
            else if (!strcmp(command, "EMPTY_TERM")){
                printf("\n***EMPTY_TERM command issued***\n");
                error = empty_term();
                if (error == ERROR_QUEUE_EMPTY){
                    printf("Could not EMPTY_TERM: No process in terminated queue\n");
                }
            }
            else if (!strcmp(command, "#")) {
                printf("\n################################################################################\n");
                printf("#%s\n", fgetstring(file));
                printf("################################################################################\n");
            }
            else {
                printf("Unrecognized command: %s\n", command);
            }
        }
        else
            if (error == EOF) {
                printf("\n");
                break;
            }
    }

    fclose(file);

    return(0);
}
