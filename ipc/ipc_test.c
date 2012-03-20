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
    char line[LINE_MAX];
    int init_num[10];
    char init_arg[50];
    char *end_temp;
    unsigned long temp_val;
    int i = 0;
    int j = 0;
    
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
                fgets(line, LINE_MAX, file);
                printf("\n!!!!!!!!!!!!!!!!\nLINE VALUE: CHECK IF PARAMETERS OF INIT\nLine: %s\n!!!!!!!!!!!!!!!!!!!!!!!\n", line);
				error = 1;
				
				/*Initialize init_num as -1*/
				for(i = 0; i < 10; i++){
					init_num[i] = -1;
				}
				
				/*Initial split of line*/
				init_arg = strtok(line, " ");
				/*If no arguments*/
				if (init_arg = NULL){
					error = 0;
				}
                while (init_arg != NULL){
					/*End pointer for strtoul*/
					end_temp = init_arg
					/*Set for strtoul*/
					errno = 0;
					/*Convert string value to unsigned long*/
					temp_val = strtoul(init_arg, &end_temp, 10);
					/*Check if errno is set off for not a number, or if more than 10 arguments*/
					if (errno != 0 || i >= 10){
						error = 0;
						break;
					}
					/*Store value*/
					init_num[i] = (int)temp_val;
					i++;
					/*Next token*/
					init_arg = strtok(NULL, " ");
				}
                
                if (error == 1){
					deinit();
					i = 0;
					while(i < 10 || init_num[i] != -1){
						init_queue(init_num[i]);
						i++;
					}
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
