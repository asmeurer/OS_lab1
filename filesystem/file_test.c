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

int main(int argc, char *argv[]) {
	int error = 0;
	char command[20];
	char line[LINE_MAX];
	int int_arg;
	int int_arg2;
	char char_arg;
	int temp;
	char* str_arg;
	char* str_arg2;
	char* init_arg;
	int j;
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
				/*Call init_fs*/
				printf("File System has been initialized.\n");
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
								/*TODO: Call format*/
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
						/*TODO: Call mount*/
						error = 0;
					}
				}
				if (error == 1){
					textcolor(BRIGHT, RED, BLACK);
					printf("Usage: MOUNT <fs_name>\n");
					textcolor(RESET, -1, -1);
				}
			}
			else if (!strcmp(command, "PAGE_FAULT")) {
				fgets(line, LINE_MAX, file);
				error = 1;
				/*Initial split of line*/
				init_arg = strtok(line, " ");
				/*If there exists arguments*/
				if (init_arg != NULL){
					error = 0;
					/*Loop through each character in string checking if it is a digit*/
					for(j = 0; j < strlen(init_arg); j++){
						if (!isdigit(init_arg[j])){
							error = 1;
							break;
						}
					}
					if (error == 0){
						int_arg = atoi(init_arg);

					}
				}
				if (error == 1){
					textcolor(BRIGHT, RED, BLACK);
					printf("Usage: PAGE_FAULT <page_table_id> <page_num>\n");
					textcolor(RESET, -1, -1);
				}
			}
			else if (!strcmp(command, "OPEN")) {
				fgets(line, LINE_MAX, file);
				error = 1;
				/*Initial split of line*/
				init_arg = strtok(line, " ");
				/*If there exists arguments*/
				if (strcmp(init_arg, "\n") != 0) {
					strcpy(str_arg, init_arg);
					
					if (!strcmp(init_arg, "USER\n")) {
						error = 0;
					}
					else if (!strcmp(init_arg, "SYSTEM\n")) {
						error = 0;
					}
					else if (!strcmp(init_arg, "BS\n")) {
						error = 0;
					}
					/* No arguments for pagetable */
					else if (!strcmp(init_arg, "PAGETABLE\n")) {
						error = 1;
					}
					else if (!strcmp(init_arg, "PAGETABLE")) {
						error = 0;
						/* Reads until end of line */
						init_arg = strtok(NULL, "\n");
						/*Loop through each character in string checking if it is a digit*/
						for(j = 0; j < strlen(init_arg); j++){
							if (!isdigit(init_arg[j])){
								error = 1;
								break;
							}
						}
						if (error == 0){
							int_arg = atoi(init_arg);
						}
					}
					else{
						error = 1;
					}
				}
				/*No arguments*/
				else{
					printf("LIST called\n");
					printf("Listing all...\n");
					error = 0;
				}
				if (error == 1){
					textcolor(BRIGHT, RED, BLACK);
					printf("Usage: LIST [USER | SYSTEM | PAGETABLE page_table_id]\n");
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
