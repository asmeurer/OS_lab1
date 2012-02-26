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

    printf("pid: %d ", process.pid);
    printf("psw: %d ", process.psw);
    printf("p_table: %d ", process.page_table);
    printf("regs:");
    for (i = 0; i < NUM_REGS; i++) {
        printf(" %d ", process.regs[i]);
    }
    printf("\n");
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
    printf("Start of %s.\n\n", enum_to_string(queue));
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
    if (temp->head == null) {
        printf("Nothing is scheduled to run (ready queue empty)\n");
    } else {
        printf("Next scheduled process PID: %d\n\n", temp->head->pid);
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

    init();


    while(1) {
        /*printf("***reading the file***\n");*/
        error = fscanf(file," %s", command);
        if (error == 1){
            /* printf("%s\n", line); */
            /* printf("%s\n", command); */

            if (!strcmp(command, "INIT")) {
                printf("\n***INIT command issued***\n");
                init();
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
                        list_Q(READY);
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
                else if (error == -2){
                    printf("Could not GO: Process already running\n");
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
            /*TODO Take out this command*/
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
                error = fscanf(file, " %d %d %d %d %d", &psw, &page_table, &reg1, &reg2, &reg3);
                if (error == 5){
                    /* printf("pid: %d, psw: %d, page_table: %d, reg1: %d, reg2: %d, reg3: %d\n", pid, psw, page_table, reg1, reg2, reg3); */
                    regs[0] = reg1;
                    regs[1] = reg2;
                    regs[2] = reg3;

                    printf("\n***CREATE command issued (PID: %d)***\n", pid_counter);
                    error = create(psw, page_table, regs);
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
                }
                else{
                    printf("Usage: CREATE <psw> <page_table> <reg1> <reg2> <reg3>\n");
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
                printf("# %s\n", fgetstring(file));
                printf("################################################################################\n");
            }
            else {
                printf("Unrecognized command: %s\n", command);
            }
        }
        else{
            printf("NOT VALID COMMAND: %s\n", command);
        }
    }

    fclose(file);

    return(0);
}
