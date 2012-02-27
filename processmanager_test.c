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

    printf("pid: %d\t", process.pid);
    printf("psw: %d\t", process.psw);
    printf("p_table: %d\t", process.page_table);
    printf("regs:");
    for (i = 0; i < NUM_REGS; i++) {
        printf(" %d", process.regs[i]);
    }
    if (scheduler == GROUP){
        printf("\tgroup: %s", enum_to_string(process.group));
    }
    else{
        printf("\tpriority: %d", process.priority);
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
                    if (!strcmp(args[0], "General_purpose")){
                        printf("\n***INIT command issued (%s)***\n", args[0]);
                        init(GROUP);
                    }
                    else if(!strcmp(args[0], "Interactive")){
                        printf("\n***INIT command issued (%s)***\n", args[0]);
                        init(PRIORITY);
                    }
                    else{
                        printf("Usage: INIT <General_purpose | Interactive>\n");
                    }
                }
                else{
                    printf("Usage: INIT <General_purpose | Interactive>\n");
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
                        list_Q(READY0);
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
                if (error == -1){
                    printf("Could not GO: No ready processes\n");
                }
                else if (error == -666){
                    printf("FATAL ERROR: SYSTEM EXIT\n");
                    exit(-1);
                }
            }
            else if (!strcmp(command, "UNWAIT")) {
                error = fscanf(file, " %d", &pid);
                if (error == 1){
                    printf("\n***UNWAIT command issued (PID: %d)***\n", pid);
                    error = unwait(pid);
                    if (error == -2){
                        printf("Could not UNWAIT: No waiting processes\n");
                    }
                    else if (error == -1){
                        printf("Could not UNWAIT: PID does not exist in waiting queue\n");
                    }
                    else if (error == -666){
                        printf("FATAL ERROR: SYSTEM EXIT\n");
                        exit(-1);
                    }
                }
                else{
                    printf("Usage: UNWAIT <pid>\n");
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
                /*Group Scheduler*/
                if (scheduler == GROUP){
                    error = fscanf(file, " %d %d %d %d %d %d", &psw, &page_table, &reg1, &reg2, &reg3, &group);
                    if (error == 6){
                        /* printf("pid: %d, psw: %d, page_table: %d, reg1: %d, reg2: %d, reg3: %d\n", pid, psw, page_table, reg1, reg2, reg3); */
                        regs[0] = reg1;
                        regs[1] = reg2;
                        regs[2] = reg3;

						
						error = create(psw, page_table, regs, group);
						
						if (error == -1 || error == -666) {
							printf("FATAL ERROR: SYSTEM EXIT\n");
							exit(-1);
						}
						else if (error == -2){
							printf("Could not CREATE: Maximum allowed processes reached\n");
						}
						else if (error == -3){
							printf("Could not CREATE: PID not unique\n");
							printf("FATAL ERROR: SYSTEM EXIT\n");
						}
						else if (error == -4){
							printf("Could not CREATE: Invalid group number\n");
						}
						else if (error == 0){
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

                        if (error == -1 || error == -666) {
                            printf("FATAL ERROR: SYSTEM EXIT\n");
                            exit(-1);
                        }
                        else if (error == -2){
                            printf("Could not CREATE: Maximum allowed processes reached\n");
                        }
                        else if (error == -3){
                            /*printf("Could not CREATE: PID not unique\n");*/
                            printf("FATAL ERROR: SYSTEM EXIT\n");
                        }
                        else if (error == -4){
                            printf("Could not CREATE: Invalid group number\n");
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
                        if (error == -1 || error == -666) {
                            printf("FATAL ERROR: SYSTEM EXIT\n");
                            exit(-1);
                        }
                        else if (error == -2){
                            printf("Could not CREATE: Maximum allowed processes reached\n");
                        }
                        else if (error == -3 || error == -4){
                            /*printf("Could not CREATE: PID not unique\n");*/
                            printf("FATAL ERROR: SYSTEM EXIT\n");
                        }
                    }
                    else{
                        printf("Usage: CREATE <psw> <page_table> <reg1> <reg2> <reg3>\n");
                    }
                }
            }
            else if (!strcmp(command, "CLEAR_TERM")){
                printf("\n***CLEAR_TERM command issued***\n");
                error = empty_term();
                if (error == -1){
                    printf("Could not CLEAR_TERM: No process in terminated queue\n");
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
