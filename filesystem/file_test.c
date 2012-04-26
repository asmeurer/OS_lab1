/**
 * Memory Test Interface
 *
 *	Group 2
 *	Aaron Meurer
 *	Oran Wallace
 *	Sheng Lundquist
 */

#include "file_test.h"

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

char parsePath(char* pathname, path* head){
	char* arg;
	path* temp;
	int len;
	char fs_name;
	arg = strtok(pathname,"/");
	len = strlen(arg);
	if (len != 1 || arg[0]<'A' || arg[0]>'Z'){
		printf("fs_name must be one uppercase character A-Z\n");
		head = NULL;
		return '!';
	}
	fs_name = arg[0];
	/*Next path segment*/
	arg = strtok(NULL, "/");
	/*Add into linked list until end of path*/
	while(arg != NULL){
		if (temp == NULL){
			temp = (path*) malloc(sizeof(path));
			head = temp;

		}
		else{
			temp->next = (path*) malloc(sizeof(path));
			temp = temp->next;
		}
		strcpy(temp->string, arg);
		arg = strtok(NULL, "/");
	}
	if (head == NULL){
		printf("Filename must be speicified\n");
		head = NULL;
		return '!';
	}
	return fs_name;
}


int strToIntArg(char* string){
	int i;
	/*Loop through each character in string checking if it is a digit*/
	for(i = 0; i < strlen(string); i++){
		if (!isdigit(string[i])){
			return ERROR_ARG_NOT_INT;
		}
	}
	return atoi(string);
}

void printPath(path* head, char fs_name){
	path* temp = head;
	printf("~%c", fs_name);
	while (temp != NULL){
		printf("/%s", temp->string);
		temp = temp->next;
	}
}

int main(int argc, char *argv[]) {
	int error = 0;
	char command[20];
	char line[LINE_MAX];
	int j;
	int int_arg;
	int int_arg2;
	char char_arg;
	int temp;
	char* str_arg;
	char* str_arg2;
	char* init_arg;
	path* head_path_arg = NULL;
	int return_error = 0;


	FILE *file;
	if (argc == 1){
		file = stdin;
		printf("The Group 2 Memory Manager Runner!\n");
		printf("Type HELP to see the list of commands\n");
	}
	else if (argc == 2){
		file = fopen(argv[1], "r");
		if (file == NULL) {
			textcolor(BRIGHT, RED, BLACK);
			printf("Could not open file %s: %s\n", argv[1], strerror(errno));
			textcolor(RESET, -1, -1);
			exit(errno);
		}
	}
	else{
		textcolor(BRIGHT, RED, BLACK);
		printf("Usage:\n./ipc_test.o (to read from stdin)\n");
		printf("./ipc_test.o <filename> (to read from filename)\n");
		textcolor(RESET, -1, -1);
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

			if (!strcmp(command, "INIT_FS")) {
				 fgets(line, LINE_MAX, file);
                error = 1;
                /*Initial split of line*/
                init_arg = strtok(line, "\n");
                /*If there exists arguments*/
                if (init_arg != NULL){
                    error = 0;
                    /*Loop through each character in string checking if it is a digit*/
                    for(j = 1; j < strlen(init_arg) - 1; j++){
                        if (!isdigit(init_arg[j])){
                            error = 1;
                            break;
                        }
                    }
                    if (error == 0){
						 int_arg = atoi(init_arg);
						 if(int_arg >= MAX_DEVICE || int_arg < 0){
							textcolor(BRIGHT, RED, BLACK);
                            printf("A device must be a number from 0-4.\n");
                            textcolor(BRIGHT, -1, -1);
						 }else{
							 init_fs(int_arg);
							 printf("File System has been initialized.\n");
							 error = 0;
						 }	 
					}
				}

			if(error == 1){
				textcolor(BRIGHT, RED, BLACK);
					printf("Usage: INIT_FS <device_num>\n");
					textcolor(RESET, -1, -1);
			}			
				
			}

			}
			else if (!strcmp(command, "FORMAT")) {
				fgets(line, LINE_MAX, file);
				error = 1;
				/*Initial split of line*/
				init_arg = strtok(line, " ");
				/*If there exists arguments*/
				if (init_arg != NULL){
					error = 0;
					int_arg = strToIntArg(init_arg);
					if(int_arg == ERROR_ARG_NOT_INT){
						printf("Device num must be an integer\n");
						error = 1;
					}
					else{
						init_arg = strtok(NULL, " ");
						/*Check size of init_arg, if more than one character, error*/
						/*-1 for null character*/
						temp = strlen(init_arg);
						if (temp != 1 || init_arg[0]<'A' || init_arg[0]>'Z'){
							printf("fs_name must be one uppercase character A-Z\n");
							error = 1;
						}
						else{
							char_arg = init_arg[0];
							/*Grab rest of line*/
							init_arg = strtok(NULL, "\n");
							int_arg2 = strToIntArg(init_arg);
							if(int_arg2 == ERROR_ARG_NOT_INT){
								printf("Block size must be an integer\n");
								error = 1;
							}
							else{
								printf("Calling FORMAT with device %d, name %c, blocksize %d\n", int_arg, char_arg, int_arg2);
								format(int_arg, char_arg, int_arg2);
								error = 0;
							}
						}
					}
				}

				if (error == 1){
					textcolor(BRIGHT, RED, BLACK);
					printf("Usage: FORMAT <device_num> <fs_name> <blocksize>\n");
					textcolor(RESET, -1, -1);
				}
			}
			else if (!strcmp(command, "MOUNT")){
				fgets(line, LINE_MAX, file);
				error = 1;
				/*Initial split of line*/
				init_arg = strtok(line, "\n");
				/*If there exists arguments*/
				if (init_arg != NULL){
					temp = strlen(init_arg);
					if (temp != 1 || init_arg[0]<'A' || init_arg[0]>'Z'){
						printf("fs_name must be one uppercase character A-Z\n");
						error = 1;
					}
					else{
						char_arg = init_arg[0];
						printf("Calling MOUNT with name %c\n", char_arg);
						mount(char_arg);
						error = 0;
					}
				}
				if (error == 1){
					textcolor(BRIGHT, RED, BLACK);
					printf("Usage: MOUNT <fs_name>\n");
					textcolor(RESET, -1, -1);
				}
			}
			
			else if(!strcmp(command, "MKDIR")) {
				fgets(line, LINE_MAX, file);
				error = 1;
				/*Initial split of line*/
				init_arg = strtok(line, " ");
				/*If there exists arguments*/
				if (strcmp(init_arg, "\n") != 0 && init_arg != NULL) {
					/*Filename*/
					str_arg = (char*) malloc(sizeof(char) * (strlen(init_arg)+1));
					strcpy(str_arg, init_arg);
					/*Option*/
					str_arg2 = strtok(NULL, "\n");
					/*If there is a second argument*/
					if(str_arg2 != NULL){						
						char_arg = parsePath(str_arg, head_path_arg);
						if (head_path_arg == NULL){
							
							error = 1;
						}else {
							
						}	
					}
				}
			}	
			else if (!strcmp(command, "OPEN")) {
				fgets(line, LINE_MAX, file);
				error = 1;
				/*Initial split of line*/
				init_arg = strtok(line, " ");
				/*If there exists arguments*/
				if (strcmp(init_arg, "\n") != 0 && init_arg != NULL) {
					/*Filename*/
					str_arg = (char*) malloc(sizeof(char) * (strlen(init_arg)+1));
					strcpy(str_arg, init_arg);
					/*Option*/
					str_arg2 = strtok(NULL, "\n");
					/*If there is a second argument*/
					if(str_arg2 != NULL){
						char_arg = parsePath(str_arg, head_path_arg);
						if (head_path_arg == NULL){

							error = 1;
						}
						else{
							if (!strcmp(str_arg2, "NEW")) {
								printf("Calling OPEN NEW on ");
								printPath(head_path_arg, char_arg);
								printf("\n");
								open(char_arg, head_path_arg, 0);	
								error = 0;
							}
							else if (!strcmp(str_arg2, "READ-ONLY")) {
								printf("Calling OPEN READ-ONLY on ");
								printPath(head_path_arg, char_arg);
								printf("\n");
								
								error = 0;
							}
							else if (!strcmp(str_arg2, "READ-WRITE")) {
								printf("Calling OPEN READ-WRITE on ");
								printPath(head_path_arg, char_arg);
								printf("\n");
								error = 0;
							}
							else{
								error = 1;
							}
						}
					}
				}
				if (error == 1){
					textcolor(BRIGHT, RED, BLACK);
					printf("Usage: OPEN <filename> [NEW | READ-ONLY | READ-WRITE]\n");
					textcolor(RESET, -1, -1);
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
				printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				printf("COMMANDS:\n");
				printf("INIT_MEM\n");
				printf("ALLOC_PT <page_table_size>\n");
				printf("DEALLOC <page_table_id>\n");
				printf("FILL_PHY_MEM <page_table_id>\n");
				printf("PAGE_HIT <page_table_id> <page_num>\n");
				printf("PAGE_FAULT <page_table_id> <page_num>\n");
				printf("LIST [USER | SYSTEM | BS | PAGETABLE page_table_id]\n");
				printf("LRU_OVERFLOW\n");
				printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			}
			else {
				textcolor(BRIGHT, RED, BLACK);
				printf("Unrecognized command: %s\n", command);
				textcolor(RESET, -1, -1);
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
