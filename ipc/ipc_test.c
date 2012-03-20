/* Test interface for IPC manager
 *
 *    Group 2
 *    Aaron Meurer
 *    Oran Wallace
 *    Sheng Lundquist
 */


#include "ipc_test.h"
#include "../shared/textcolor.c"
#include "ipc_definitions.h"
#include "ipc_queuemanager.h"
#include "ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printmessage(struct message MESSAGE){
    printf("message: ");
}

/*
**
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
    int choice[10];
    char command[20];
    char dest[15];
    char source[15];
    char message[256];
    char args[10][10];
    char line[80];
    FILE *file;
    if (argc == 1){
        file = stdin;
    }
    else if (argc == 2){
        file = fopen(argv[1], "r");
    }
    else{
        printf("Usage:\n./ipc_test.o (to read from stdin)\n");
        printf("./ipc_test.o <filename> (to read from filename)\n");
        exit(-1);
    }


    while(1) {
        /*printf("***reading the file***\n");*/
        if (file == stdin) {
            printf("> ");
        }
        error = fscanf(file," %s", command);
        if (error == 1){
            /* printf("%s\n", line); */
            /* printf("%s\n", command); */

            if (!strcmp(command, "INIT_IPC")) {
                error = fscanf(file, " %s", args[0]);

                if (error == 1){
                    init();
                }
                else{
                    printf("Usage: INIT <manager1 | manager2| ...| managerN >>\n");
                }

            }
            else if (!strcmp(command, "LIST")) {
                error = fscanf(file, " %s", args[0]);
                if (error == 1){
                    printf("\n***LISTING command issued (%s)***\n", args[0]);
                    if (!strcmp(args[0], "ZERO")){

                    }
                    else if (!strcmp(args[0], "ONE")){

                    }
                    else if (!strcmp(args[0], "TWO")){

                    }
                    else if (!strcmp(args[0], "THREE")){

                    }
                    else if (!strcmp(args[0], "FOUR")){

                    }
                    else if (!strcmp(args[0], "FIVE")){

                    }
                    else if (!strcmp(args[0], "SIX")){

                    }
                    else if (!strcmp(args[0], "SEVEN")){

                    }
                    else if (!strcmp(args[0], "EIGHT")){

                    }
                    else if (!strcmp(args[0], "NINE")){

                    }
                    else{
                        printf("Usage: LIST <queuename>\n");
                    }
                }
                else{
                    printf("Usage: LIST <queuename>\n");
                }
            }
            else if (!strcmp(command, "HAS_MESSAGE")){
                error = fscanf(file, " %s", &dest);
                if(error == 1){
                    printf("\n***HAS_MESSAGE command issued***");
                    error = has_message(atoi(dest));
                    if(error == ERROR_QUEUE_EMPTY){
                        printf("FALSE");
                    }
                    else if(error == ERROR_QUEUE_NOT_EXIST){
                        printf("Queue specified does not exist");
                    }
                    else if(error == ERROR_SUCCESS){
                        printf("TRUE");
                    }
                }else{
                    printf("Usage: HAS_MESSAGE <queuename> ");
                }
            }

            else if (!strcmp(command, "SEND")) {
                error = fscanf(file, " %s %s %s", &source, &dest, &message);
                if (error == 1){
                    printf("\n***SEND command issued ***\n");

                    error = send(atoi(source), atoi(dest), message);
                    if(error == ERROR_QUEUE_FULL){
                        printf("Destination queue full");
                    }
                    else if(error == ERROR_QUEUE_NOT_EXIST){
                        printf("Destination queue does not exist");
                    }
                    else if(error == ERROR_MAX_STRING_LENGTH){
                        printf("The message length was too long.");
                    }
                }
                else{
                    printf("Usage: SEND <source destination message>\n");
                }
            }
            else if (!strcmp(command, "RETRIEVE")) {
                error = fscanf(file,"  %s", &dest);
                if(error == 1){
                    printf("\n***RETRIEVE command issued ***\n");
                    /*error = retrieve(dest); */
                    if(error == ERROR_QUEUE_EMPTY){
                        printf("Queue does not exist");
                    }
                    else if(error == ERROR_QUEUE_NOT_EXIST){
                        printf("Queue does not exist");
                    }
                }
                else{
                    printf("Usage: RETRIEVE <destination> ");

                }
            }
            else if (!strcmp(command, "#")) {
                printf("\n################################################################################\n");
                printf("#");
                textcolor(BRIGHT, WHITE, BLACK);
                printf("%s\n", fgetstring(file));
                textcolor(RESET, -1, -1);
                printf("################################################################################\n");
            }
            else if(!strcmp(command, "HELP")){
                printf("\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                printf("COMMANDS: INIT_IPC <manager1 | manager2 | ... | managerN>    LIST <queuename>\n");
                printf("HAS_MESSAGE <queuename>     SEND <source destination message>\n");
                printf("RETRIEVE <destination> \n");
                printf("\n ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            }
            else {
                printf("Unrecognized command: %s\n", command);
            }
        }
        else if (error == EOF) {
            printf("\n");
            break;
        }
    }

    fclose(file);

    return(0);
}
