/* The test file!

   Aaron Meurer
   Oran Wallace
   Sheng Lundquist
*/

#include<stdio.h>
#include<string.h>
#include "queuemanager.c"

void printprocess(struct process_control_block process);
void listQ();

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

void listQ() {
    struct process_control_block *temp = head;
    printf("Start of queue.\n");
    while (temp) {
        printprocess(*temp);
        temp = temp->prev;
    }
    printf("\nEnd of queue.\n\n");
}

int main() {
    int i = 0;
    int r = 0;
    char line[100];
    char delim[] = " \n";
    char *command;
    char *args[6];
    int pid = 0;
    int psw = 0;
    int page_table = 0;
    int reg1 = 0;
    int reg2 = 0;
    int reg3 = 0;
    int regs[3];

    init();

    FILE *file = fopen("tests", "r");

    while(fgets(line, 100, file) != NULL) {
        /* printf("reading the file\n"); */
        command = strtok(line, delim);

        /* printf("%s\n", line); */
        /* printf("%s\n", command); */

        if (!strcmp(command, "init_Q")) {
            /* printf("initializing\n"); */
            init();
        }
        else if (!strcmp(command, "list")) {
            /* printf("listing\n"); */
            listQ();
        }
        else if (!strcmp(command, "enqueue")) {
            /* printf("enqueueing\n"); */
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

            r = enqueue(pid, psw, page_table, regs);
            if (r == -1) {
                printf("Could not enqueue; queue full.\n");
            }
        }
        else if (!strcmp(command, "dequeue")) {
            /* printf("dequeueing\n"); */
            r = dequeue();
            if (r == -1) {
                printf("Could not dequeue: queue empty.\n");
            } else {
                printf("%d\n", r);
            }
        }
        else if (!strcmp(command, "delete")) {
            /* printf("deleting\n"); */
            pid = atoi(strtok(NULL, delim));
            r = delete(pid);
            if (r == -1) {
                printf("Could not dequeue: queue empty.");
            } else {
                printf("%d\n", r);
            }
        }
        else {
            printf("Unrecognized command: %s\n", command);
        }
    }

    close(file);

    return(0);
}
