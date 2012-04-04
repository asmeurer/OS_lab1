/**
* Memory Test Interface
*
*	Group 2
*	Aaron Meurer
*	Oran Wallace
*	Sheng Lundquist
*/

#include "memory_test.h"

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

int main(int argc, char *argv[]) {
	int error = 0;
	char command[20];
	char line[LINE_MAX];
	char *init_arg;
    
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

			if (!strcmp(command, "INIT_MEM")) {

			}
			else if (!strcmp(command, "ALLOC_PT")) {

			}
			else if (!strcmp(command, "DEALLOC_PT")){

			}
			else if (!strcmp(command, "PAGE_FAULT")) {

			}
			else if (!strcmp(command, "LIST")) {
				fgets(line, LINE_MAX, file);
				error = 1;
				/*Initial split of line*/
				init_arg = strtok(line, " ");
				/*If there exists arguments*/
				if (strcmp(init_arg, "\n") != 0){

				}
				else{
					textcolor(BRIGHT, RED, BLACK);
					printf("Usage: LIST <manager1 | manager2| ...| managerN >\n");
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

