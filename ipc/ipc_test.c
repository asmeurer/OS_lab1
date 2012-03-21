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
#include <ctype.h>


/**
 * Function to print a queue.
 * @param queuelist The queue to list out.
 */
void list_MQ(enum MESSAGE_QUEUES queuelist){
    struct queue_message_t *structqueue = get_message(queuelist);
    if (structqueue->initialized == 0){
		printf("\nQueue %d Not Initialized.\n", queuelist);
		return;
	}

    struct message *temp = structqueue->head;
    printf("\nStart of message queue %d.\n", queuelist);

    while(temp){
        printmessage(*temp);
        temp = temp->prev;
    }
    printf("End of message queue %d.\n", queuelist);
}

void list_all_MQ(){
    int i=0;
    for(i=0;i < 10; i++){
        list_MQ(i);
    }
}


/**
 * Function to print out a specific message block.
 * @param MESSAGE The message to be printed.
 */
void printmessage(struct message MESSAGE){
    printf("source: %d  |  ", MESSAGE.source);
    printf("destination: %d  |  ", MESSAGE.destination);
    printf("message: %s  |\n", MESSAGE.string);
}


/**
 * Gets a string from the file.
 * @param fFile The file to read the string from.
 * @return The string that was in the file.
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



/**
 * The main function.
 * @param argc The number of command line arguments.
 * @param argv The array of arguments.
 * @return Returns 0.
 */
int main(int argc, char *argv[]) {
    int error = 0;
    char command[20];
    int dest;
    int source;
    char message[256];
    char line[LINE_MAX];
    int init_num[10];
    char *init_arg;
    char *end_temp;
    unsigned long temp_val;
    int i = 0;
    int j = 0;

    FILE *file;
    if (argc == 1){
        file = stdin;
        printf("Type HELP to see the list of commands\n");
    }
    else if (argc == 2){
        file = fopen(argv[1], "r");
        if (file == NULL) {
            printf("Could not open file %s: %s\n", argv[1], strerror(errno));
            exit(errno);
        }
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
                error = 1;

                /*Initialize init_num as -1*/
                for(i = 0; i < 10; i++){
                    init_num[i] = -1;
                }

                /*Initial split of line*/
                init_arg = strtok(line, " ");
                /*If no arguments*/
                if (strcmp(init_arg, "\n") == 0){
                    error = 0;
                }
                i = 0;
                while (init_arg != NULL) {
                    /*End pointer for strtoul*/
                    end_temp = init_arg;

                    /*Loop through each character in string checking if it is a digit*/
                    for(j = 0; j < strlen(init_arg) - 1; j++){
						if (!isdigit(init_arg[j])){
							error = 0;
							break;
						}
					}
                    if(error == 0){
						break;
					}


                    /*Set for strtoul*/
                    errno = 0;
                    /*Convert string value to unsigned long*/
                    temp_val = strtoul(init_arg, &end_temp, 10);
                    /*Check if errno is set off for not a number, or if more than 10 arguments*/
                    if (errno == EINVAL || i >= 10){
                        error = 0;
                        break;
                    }
                    /*Check for more than 10 arguments*/
                    if (temp_val >= 10){
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
                    /*The first -1 should be where it stops*/
                    while(i < 10 && init_num[i] != -1){

                        printf("Message Queue %d initialized.\n", init_num[i]);
                        init_queue(init_num[i]);
                        i++;
                    }
                    printf("\n");

                }
                else{
                    printf("Usage: INIT_IPC <manager1 | manager2| ...| managerN >\n");
                }

            }
            else if (!strcmp(command, "LIST")) {
				fgets(line, LINE_MAX, file);
                error = 1;

                /*Initialize init_num as -1*/
                for(i = 0; i < 10; i++){
                    init_num[i] = -1;
                }

                /*Initial split of line*/
                init_arg = strtok(line, " ");
                /*If no arguments*/
                if (strcmp(init_arg, "\n") == 0){
                    list_all_MQ();
                }
                else{
					i = 0;
					while (init_arg != NULL) {
						/*End pointer for strtoul*/
						end_temp = init_arg;

						/*Loop through each character in string checking if it is a digit*/
						for(j = 0; j < strlen(init_arg) - 1; j++){
							if (!isdigit(init_arg[j])){
								error = 0;
								break;
							}
						}
						if(error == 0){
							break;
						}


						/*Set for strtoul*/
						errno = 0;
						/*Convert string value to unsigned long*/
						temp_val = strtoul(init_arg, &end_temp, 10);
						/*Check if errno is set off for not a number, or if more than 10 arguments*/
						if (errno == EINVAL || i >= 10){
							error = 0;
							break;
						}
						/*Check for more than 10 arguments*/
						if (temp_val >= 10){
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
						i = 0;
						/*The first -1 should be where it stops*/
						while(i < 10 && init_num[i] != -1){
							list_MQ(init_num[i]);
							i++;
						}
						printf("\n");
					}
					else{
						printf("Usage: LIST || LIST <manager1 | manager2| ...| managerN >\n");
					}
				}
            }
            else if (!strcmp(command, "HAS_MESSAGE")){
                error = fscanf(file, " %d", &dest);
                if(error == 1){
                    printf("\n***HAS_MESSAGE command issued***\n");
                    error = has_message(dest);
                    if(error == ERROR_QUEUE_EMPTY){
                        printf("FALSE\n");
                    }
                    else if(error == ERROR_DEST_QUEUE_NOT_EXIST){
                        printf("Queue specified does not exist\n");
                    }
                    else if(error == ERROR_SUCCESS){
                        printf("TRUE\n");
                    } else {
                        printf("Unexpected error in HAS_MESSAGE (%d)!\n", error);
                    }
                }else{
                    printf("Usage: HAS_MESSAGE <queuename> \n");
                }
            }

            else if (!strcmp(command, "SEND")) {
                error = fscanf(file, " %d %d %[^\n]s", &source, &dest, message);
                if (error == 3){
                    printf("\n***SEND command issued ***\n");
                    printf("message: %s\n", message);

                    error = send(source, dest, message);
                    if (error == ERROR_SUCCESS) {
                        printf("Send successful\n");
                    } else if(error == ERROR_QUEUE_FULL){
                        printf("Destination queue full\n");
                    }
                    else if (error == ERROR_SOURCE_QUEUE_NOT_EXIST) {
                        printf("Source queue does not exist\n");
                    }
                    else if(error == ERROR_DEST_QUEUE_NOT_EXIST){
                        printf("Destination queue does not exist\n");
                    }
                    else if(error == ERROR_MAX_STRING_LENGTH){
                        printf("The message length was too long.\n");
                    }
                    else {
                        printf("Unexpected error in SEND (%d)!\n", error);
                    }

                }
                else{
                    printf("Usage: SEND <source destination message>\n");
                }
            }
            else if (!strcmp(command, "RETRIEVE")) {
                error = fscanf(file,"  %d", &dest);
                if(error == 1){
                    printf("\n***RETRIEVE command issued ***\n");
                    error = retrieve(dest, message);

                    if (error == ERROR_SUCCESS) {
                        printf("Message retrieved successfully.\n");
                        printf("%s\n", message);
                    } else if (error == ERROR_QUEUE_EMPTY){
                        printf("Queue does not exist\n");
                    }
                    else if(error == ERROR_DEST_QUEUE_NOT_EXIST){
                        printf("Queue does not exist\n");
                    } else {
                        printf("Unexpected Error RETRIEVE (%d)!\n", error);
                    }
                }
                else{
                    printf("Usage: RETRIEVE <destination>\n ");

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
                printf("COMMANDS: INIT_IPC <manager1 | manager2 | ... | managerN>\n");
                printf("LIST <queuename>\n");
                printf("HAS_MESSAGE <queuename>\n");
                printf("SEND <source destination message>\n");
                printf("RETRIEVE <destination>\n");
                printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
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
