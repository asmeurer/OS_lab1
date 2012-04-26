/**
 * Memory Test Interface
 *
 *    Group 2
 *    Aaron Meurer
 *    Oran Wallace
 *    Sheng Lundquist
 */

#include "file_test.h"

void list_devices(){
    int i;
    printf("Listing devices:\n");
    for(i = 0; i < MAX_DEVICE; i++){
        /*If device is initialized*/
        if (device_array[i].bits & DEVICE_INIT_BITMASK){
            printf("Device number: %d    ", i);
            printf("Formated: ");
            if(device_array[i].bits & DEVICE_FORMAT_BITMASK){
                printf("Yes    Fs_name: %c    ", device_array[i].fs_name);
            }
            else{
                printf("No    Fs_name: -    ");
            }
            printf("Mounted: ");
            if(device_array[i].bits & DEVICE_MOUNTED_BITMASK){
                printf("Yes");
            }
            else{
                printf("No");
            }
            printf("\n");
        }
    }
}

int list_fileinfo(char fs_name, path *file_path){
    fcb* temp;
    block* temp2;
    int dev = get_device(fs_name);
    fcb *file = get_file(dev, file_path);
    if (dev < 0) {
        /* There was an error in get_device() */
        return dev;
    }
    if (file->error < 0) {
        /* There was an error with get_file() */
        return file->error;
    }
    printf("Listing file ");
    printPath(file_path, fs_name);
    printf("\n");
    printf("Filename: %s    ", file->filename);
    printf("Type: ");
    if(file->bits & FCB_DIR_BITMASK){
        printf("Directory    # of files: %d\n", file->dirHead->size);
        printf("Listing files in directory:\n");
        temp = file->dirHead->tail;
        if (temp == NULL){
            printf("Directory empty\n");
        }
        else{
            while(temp != NULL){
                printf("Filename: %s    ", temp->filename);
                printf("Type: ");
                if(temp->bits & FCB_DIR_BITMASK){
                    printf("Directory");
                }
                else{
                    printf("File");
                }
                printf("\n");
                temp = temp->next;
            }
        }
    }
    else{
        printf("File\n");
        printf("Size: %d\n", file->block_queue->size);
        temp2 = file->block_queue->tail;
        if (temp2 == NULL){
            printf("File empty\n");
        }
        else{
            while(temp2 != NULL){
                printf("Block Addr: %d\n", temp2->addr);
                temp2 = temp2->next;
            }
        }
    }
    return ERROR_SUCCESS;
}

void print_tabs(int count)
{
    int i;
    for (i = 0; i < count; i++) {
        printf("\t");
    }
}

void listRec(fcb* file, int count){
    fcb* temp;
    int i = 1;

    /*If it is a directory*/
    if(file->bits & FCB_DIR_BITMASK){
        printf("Directory %s, %d files:\n", file->filename, file->dirHead->size);
        temp = file->dirHead->tail;
        while(temp != NULL){
            print_tabs(count);
            printf("%-2d ", i);
            listRec(temp, count + 1);
            temp = temp->next;
            i++;
        }
        print_tabs(count - 1);
        printf("End of directory %s\n", file->filename);
    }
    else{
        printf("Filename: %s    Size: %d\n", file->filename, file->block_queue->size);
    }
}

void listDirectory(){
    int i = 0;
    for(i = 0; i < MAX_DEVICE; i++){
        if(device_array[i].bits & DEVICE_INIT_BITMASK &&
           device_array[i].bits & DEVICE_FORMAT_BITMASK &&
           device_array[i].bits & DEVICE_MOUNTED_BITMASK){
            printf("Listing device %c\n", device_array[i].fs_name);
            listRec(device_array[i].root, 0);
            printf("End of device %c\n", device_array[i].fs_name);
        }
    }

}

void listBuffers(){
	int i;
	int j;
	for (i = 0; i < NUM_BUFFERS; i++){
		printf("Buffer number %d\n", i);
		for (j = 0; j < BUFFER_SIZE; j++){
			if(buffers[i][j].init == 0){
				break;
			}
			else{
				printf("%d:	Device: %d	Addr: %d	Access Type: ",j + 1, buffers[i][j].device_num, buffers[i][j].addr);
				if(buffers[i][j].access_type == READ){
					printf("Read");
				}
				else{
					printf("Write");
				}
				printf("\n");
			}
		}
		printf("End buffer %d\n", i);
	}
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

path* parsePath(char* pathname, char* fs){
    char* arg;
    path* temp = NULL;
    path* head = NULL;
    int len;
    char fs_name;
    arg = strtok(pathname,"/");
    len = strlen(arg);
    if (len != 1 || arg[0]<'A' || arg[0]>'Z'){
        printf("fs_name must be one uppercase character A-Z\n");
        return NULL;
    }
    fs_name = arg[0];
    /*Next path segment*/
    arg = strtok(NULL, "/");
    /*If nothing after fs_name*/
    if (arg == NULL){
        printf("Must specify filename\n");
        return NULL;
    }
    /*Add into linked list until end of path*/
    while(arg != NULL){
        if (strlen(arg) >= NAME_LIMIT) {
            textcolor(BRIGHT, RED, BLACK);
            printf("File name too long (must be no more than %d characters).\n", NAME_LIMIT - 1);
            textcolor(BRIGHT, -1, -1);

            return NULL;
        }
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
    temp->next = NULL;
    *fs = fs_name;
    return head;
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
    printf("%c", fs_name);
    while (temp != NULL){
        printf("/%s", temp->string);
        temp = temp->next;
    }
}

void errorToString(int error, char* command){
    textcolor(BRIGHT, RED, BLACK);
    printf("Error in %s, code %d: ", command, error);
    textcolor(RESET, -1, -1);
    switch(error){
    case ERROR_INVALID_DEVICE_NUM:
        printf("Device number not valid\n");
        break;
    case ERROR_DEVICE_MOUNTED:
        printf("Device mounted\n");
        break;
    case ERROR_FILE_NOT_OPEN:
        printf("File is not open\n");
        break;
    case ERROR_BLOCK_NOT_IN_FILE:
        printf("Block is not in file\n");
        break;
    case ERROR_BUFFER_FULL:
        printf("Buffer is full\n");
        break;
    case ERROR_BUFFER_NOT_EXIST:
        printf("Buffer does not exist\n");
        break;
    case ERROR_NOT_INITIALIZED_OR_FORMATED:
        printf("Device is not initialized or formatted\n");
        break;
    case ERROR_DEVICE_NOT_KNOWN:
        printf("Device is not known\n");
        break;
    case ERROR_SOURCE_QUEUE_NOT_EXIST:
        printf("Source Queue does not exist\n");
        break;
    case ERROR_BAD_FILE_PTR:
        printf("Bad file pointer\n");
        break;
    case ERROR_BAD_DIR_QUEUE:
        printf("Bad directory queue\n");
        break;
    case ERROR_DIR_QUEUE_EMPTY:
        printf("Directory queue empty\n");
        break;
    case ERROR_ARG_NOT_INT:
        printf("Argument is not an integer\n");
        break;
    case ERROR_BAD_BLOCK_PTR:
        printf("Bad block pointer\n");
        break;
    case ERROR_BAD_BLOCK_QUEUE:
        printf("Bad block queue\n");
        break;
    case ERROR_BLOCK_QUEUE_EMPTY:
        printf("Block queue empty\n");
        break;
    case ERROR_BAD_FS_NAME:
        printf("Bad file system name\n");
        break;
    case ERROR_FILE_ALREADY_EXISTS:
        printf("File already exists\n");
        break;
    case ERROR_DIR_IS_FILE:
        printf("Directory is a file\n");
        break;
    case ERROR_DIR_NOT_FOUND:
        printf("Directory is not found\n");
        break;
    case ERROR_INVALID_BLOCK_SIZE:
        printf("Invalid block size\n");
        break;
    case ERROR_FILE_IS_DIR:
        printf("File is a directory\n");
        break;
    case ERROR_FS_NAME_ARG:
        printf("File system arguement not valid\n");
        break;
    case ERROR_BAD_OPTION:
        printf("Bad option\n");
        break;
    case ERROR_TOO_MANY_OPEN_FILES:
        printf("Too many open files\n");
        break;
    case ERROR_NO_FREE_BLOCKS:
        printf("No free blocks\n");
        break;
    case ERROR_ADDR_OUT_OF_BOUNDS:
        printf("Address out of bounds\n");
        break;
    case ERROR_BLOCK_ALREADY_EMPTY:
        printf("Block already empty\n");
        break;
    case ERROR_BLOCK_ALREADY_FULL:
        printf("Block already full\n");
        break;
    case ERROR_FILES_ARE_OPEN:
        printf("Files are open\n");
        break;
    case ERROR_ALREADY_MOUNTED:
        printf("Device already mounted\n");
        break;
    case ERROR_ALREADY_UNMOUNTED:
        printf("Device already unmounted\n");
        break;
    case ERROR_FS_NAME_NOT_EXISTS:
        printf("File system name does not exist\n");
        break;
    case ERROR_FILE_IS_READ_ONLY:
        printf("File is read only\n");
        break;
    case ERROR_FILE_HANDLE_OUT_OF_RANGE:
        printf("File handle is out of range\n");
        break;
    case ERROR_FILE_NOT_FOUND:
        printf("File not found\n");
        break;
    case ERROR_FILE_ALREADY_OPEN:
        printf("File is already open\n");
        break;
    default:
        printf("Unknown error\n");
    }
}

int main(int argc, char *argv[]) {
    int error = 0;
    char command[20];
    char line[LINE_MAX];
    int int_arg;
    int int_arg2;
    int int_arg3;
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
                if (init_arg != NULL && strcmp(init_arg, "\n") != 0){
                    /*Skip space*/
                    init_arg++;
                    int_arg = strToIntArg(init_arg);
                    if(int_arg == ERROR_ARG_NOT_INT){
                        printf("Device must be an integer\n");
                        error = 1;
                    }
                    else{
                        if(int_arg >= MAX_DEVICE || int_arg < 0){
                            textcolor(BRIGHT, RED, BLACK);
                            printf("A device must be a number from 0-4.\n");
                            textcolor(BRIGHT, -1, -1);
                            error = 1;
                        }else{
                            printf("Calling init_fs with device %d\n", int_arg);
                            return_error=init_fs(int_arg);
                            if(return_error < 0){
                                errorToString(return_error, "INIT_FS");
                            }
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
                                return_error = _format(int_arg, char_arg, int_arg2);
                                if(return_error < 0){
                                    errorToString(return_error, "FORMAT");
                                }
                                else{
                                    printf("Success, device has %d blocks\n", return_error);
                                }
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
                    init_arg++;
                    temp = strlen(init_arg);
                    if (temp != 1 || init_arg[0]<'A' || init_arg[0]>'Z'){
                        printf("fs_name must be one uppercase character A-Z\n");
                        error = 1;
                    }
                    else{
                        char_arg = init_arg[0];
                        printf("Calling MOUNT with name %c\n", char_arg);
                        return_error = mount(char_arg);
                        if(return_error < 0){
                            errorToString(return_error, "MOUNT");
                        }
                        else{
                            printf("Success\n");
                        }
                        error = 0;
                    }
                }
                if (error == 1){
                    textcolor(BRIGHT, RED, BLACK);
                    printf("Usage: MOUNT <fs_name>\n");
                    textcolor(RESET, -1, -1);
                }
            }

            else if (!strcmp(command, "UNMOUNT")){
                fgets(line, LINE_MAX, file);
                error = 1;
                /*Initial split of line*/
                init_arg = strtok(line, "\n");
                /*If there exists arguments*/
                if (init_arg != NULL){
                    init_arg++;
                    temp = strlen(init_arg);
                    if (temp != 1 || init_arg[0]<'A' || init_arg[0]>'Z'){
                        printf("fs_name must be one uppercase character A-Z\n");
                        error = 1;
                    }
                    else{
                        char_arg = init_arg[0];
                        printf("Calling UNMOUNT with name %c\n", char_arg);
                        return_error = unmount(char_arg);
                        if(return_error < 0){
                            errorToString(return_error, "UNMOUNT");
                        }
                        else{
                            printf("Success\n");
                        }
                        error = 0;
                    }
                }
                if (error == 1){
                    textcolor(BRIGHT, RED, BLACK);
                    printf("Usage: UNMOUNT <fs_name>\n");
                    textcolor(RESET, -1, -1);
                }
            }

            else if(!strcmp(command, "MKDIR")) {
                fgets(line, LINE_MAX, file);
                error = 1;
                /*Initial split of line*/
                init_arg = strtok(line, "\n");
                /*If there exists arguments*/
                if (strcmp(init_arg, "\n") != 0 && init_arg != NULL) {
                    init_arg++;
                    head_path_arg = parsePath(init_arg, &char_arg);
                    if (head_path_arg == NULL){
                        error = 0;
                    }else{
                        printf("Calling MKDIR on ");
                        printPath(head_path_arg, char_arg);
                        printf("\n");
                        return_error = create(char_arg, head_path_arg, 1);
                        if(return_error < 0){
                            errorToString(return_error, "MKDIR");
                        }
                        else{
                            printf("Success\n");
                        }
                        error = 0;
                    }
                }
                if(error == 1){
                    textcolor(BRIGHT, RED, BLACK);
                    printf("Usage: MKDIR <dirname>\n");
                    textcolor(RESET, -1, -1);
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
                        head_path_arg = parsePath(str_arg, &char_arg);
                        if (head_path_arg == NULL){
                            error = 1;
                        }
                        else{
                            if (!strcmp(str_arg2, "NEW")) {
                                printf("Calling OPEN NEW on ");
                                printPath(head_path_arg, char_arg);
                                printf("\n");
                                return_error = create(char_arg, head_path_arg, 0);
                                if(return_error < 0){
                                    errorToString(return_error, "OPEN NEW");
                                }
                                else{
                                    printf("Success\n");
                                }
                                error = 0;
                            }
                            else if (!strcmp(str_arg2, "READ-ONLY")) {
                                printf("Calling OPEN READ-ONLY on ");
                                printPath(head_path_arg, char_arg);
                                printf("\n");
                                return_error = open(char_arg, head_path_arg, 0);
                                if(return_error < 0){
                                    errorToString(return_error, "OPEN READ-ONLY");
                                }
                                else{
                                    printf("Success, file handle %d\n", return_error);
                                }
                                error = 0;
                            }
                            else if (!strcmp(str_arg2, "READ-WRITE")) {
                                printf("Calling OPEN READ-WRITE on ");
                                printPath(head_path_arg, char_arg);
                                printf("\n");
                                return_error = open(char_arg, head_path_arg, 1);
                                if(return_error < 0){
                                    errorToString(return_error, "OPEN READ-WRITE");
                                }
                                else{
                                    printf("Success, file handle %d\n", return_error);
                                }
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
            else if (!strcmp(command, "READ")) {
                fgets(line, LINE_MAX, file);
                error = 1;
                /*Initial split of line*/
                init_arg = strtok(line, " ");
                /*If there exists arguments*/
                if (init_arg != NULL && strcmp(init_arg, "\n") != 0){
                    int_arg = strToIntArg(init_arg);
                    if(int_arg == ERROR_ARG_NOT_INT){
                        printf("File handle must be an integer\n");
                        error = 1;
                    }
                    else{
                        init_arg = strtok(NULL, " ");
                        int_arg2 = strToIntArg(init_arg);
                        if(int_arg2 == ERROR_ARG_NOT_INT){
                            printf("Block number must be an integer\n");
                            error = 1;
                        }
                        else{
                            init_arg = strtok(NULL, "\n");
                            int_arg3 = strToIntArg(init_arg);
                            if(int_arg3 == ERROR_ARG_NOT_INT){
                                printf("Buf_ptr must be an integer\n");
                                error = 1;
                            }
                            else{
                                printf("Called READ on filehandle %d, block number %d, buf_ptr %d\n", int_arg, int_arg2, int_arg3);
                                return_error = read(int_arg, (short)int_arg2, int_arg3);
                                if(return_error < 0){
                                    errorToString(return_error, "READ");
                                }
                                else{
                                    printf("Success\n");
                                }
                                error = 0;
                            }
                        }
                    }
                }
                if (error == 1){
                    textcolor(BRIGHT, RED, BLACK);
                    printf("Usage: READ <filehandle> <block_number> <buf_ptr>\n");
                    textcolor(RESET, -1, -1);
                }
            }

            else if (!strcmp(command, "WRITE")) {
                fgets(line, LINE_MAX, file);
                error = 1;
                /*Initial split of line*/
                init_arg = strtok(line, " ");
                /*If there exists arguments*/
                if (init_arg != NULL && strcmp(init_arg, "\n") != 0){
                    int_arg = strToIntArg(init_arg);
                    if(int_arg == ERROR_ARG_NOT_INT){
                        printf("File handle must be an integer\n");
                        error = 1;
                    }
                    else{
                        init_arg = strtok(NULL, " ");
                        int_arg2 = strToIntArg(init_arg);
                        if(int_arg2 == ERROR_ARG_NOT_INT){
                            printf("Block number must be an integer\n");
                            error = 1;
                        }
                        else{
                            init_arg = strtok(NULL, "\n");
                            int_arg3 = strToIntArg(init_arg);
                            if(int_arg3 == ERROR_ARG_NOT_INT){
                                printf("Buf_ptr must be an integer\n");
                                error = 1;
                            }
                            else{
                                printf("Called WRITE on filehandle %d, block number %d, buf_ptr %d\n", int_arg, int_arg2, int_arg3);
                                return_error = write(int_arg, (short)int_arg2, int_arg3);
                                if(return_error < 0){
                                    errorToString(return_error, "WRITE");
                                }
                                else{
                                    printf("Success\n");
                                }
                                error = 0;
                            }
                        }
                    }
                }
                if (error == 1){
                    textcolor(BRIGHT, RED, BLACK);
                    printf("Usage: WRITE <filehandle> <block_number> <buf_ptr>\n");
                    textcolor(RESET, -1, -1);
                }
            }

            else if (!strcmp(command, "CLOSE")) {
                fgets(line, LINE_MAX, file);
                error = 1;
                /*Initial split of line*/
                init_arg = strtok(line, "\n");
                /*If there exists arguments*/
                if (init_arg != NULL && strcmp(init_arg, "\n") != 0){
                    /*Skip space*/
                    init_arg++;
                    int_arg = strToIntArg(init_arg);
                    if(int_arg == ERROR_ARG_NOT_INT){
                        printf("File handle must be an integer\n");
                        error = 1;
                    }
                    else{
                        printf("Calling close on %d\n", int_arg);
                        return_error = close(int_arg);
                        if(return_error < 0){
                            errorToString(return_error, "CLOSE");
                        }
                        else{
                            printf("Success\n");
                        }
                        error = 0;
                    }
                }
                if(error == 1){
                    textcolor(BRIGHT, RED, BLACK);
                    printf("Usage: CLOSE <file_handle>\n");
                    textcolor(RESET, -1, -1);
                }
            }

            else if(!strcmp(command, "DELETE")) {
                fgets(line, LINE_MAX, file);
                error = 1;
                /*Initial split of line*/
                init_arg = strtok(line, "\n");
                /*If there exists arguments*/
                if (strcmp(init_arg, "\n") != 0 && init_arg != NULL) {
                    /*Skip space*/
                    init_arg++;
                    head_path_arg = parsePath(init_arg, &char_arg);
                    if (head_path_arg == NULL){
                        error = 1;
                    }else{
                        printf("Calling DELETE on ");
                        printPath(head_path_arg, char_arg);
                        printf("\n");
                        return_error = delete(char_arg, head_path_arg);
                        if(return_error < 0){
                            errorToString(return_error, "DELETE");
                        }
                        else{
                            printf("Success\n");
                        }

                        error = 0;
                    }
                }
                if(error == 1){
                    textcolor(BRIGHT, RED, BLACK);
                    printf("Usage: DELETE <filepath>\n");
                    textcolor(RESET, -1, -1);
                }
            }

            else if (!strcmp(command, "BUF_FLUSH")) {
                fgets(line, LINE_MAX, file);
                error = 1;
                /*Initial split of line*/
                init_arg = strtok(line, "\n");
                /*If there exists arguments*/
                if (init_arg != NULL && strcmp(init_arg, "\n") != 0){
                    /*Skip space*/
                    init_arg++;
                    int_arg = strToIntArg(init_arg);
                    if(int_arg == ERROR_ARG_NOT_INT){
                        printf("buf_ptr must be an integer\n");
                        error = 1;
                    }
                    else{
                        printf("Calling buf_flush on %d\n", int_arg);
                        return_error = buf_flush(int_arg);
                        if(return_error < 0){
                            errorToString(return_error, "BUF_FLUSH");
                        }
                        else{
                            printf("Success\n");
                        }
                        error = 0;
                    }
                }
                if(error == 1){
                    textcolor(BRIGHT, RED, BLACK);
                    printf("Usage: BUF_FLUSH <buf_ptr>\n");
                    textcolor(RESET, -1, -1);
                }

            }
            else if (!strcmp(command, "LIST")) {
                fgets(line, LINE_MAX, file);
                error = 1;
                /*Initial split of line*/
                init_arg = strtok(line, " ");
                /*If there exists arguments*/
                if (strcmp(init_arg, "\n") != 0) {
                    if (!strcmp(init_arg, "DEVICES\n")) {
                        printf("Calling LIST DEVICES\n");
                        list_devices();
                        error = 0;
                    }
                    /*fileinfo has no parameter*/
                    else if (!strcmp(init_arg, "FILEINFO\n")){
                        error = 1;
                    }
                    else if (!strcmp(init_arg, "FILEINFO")) {
                        init_arg = strtok(NULL, "\n");
                        head_path_arg = parsePath(init_arg, &char_arg);
                        if (head_path_arg == NULL){
                            error = 1;
                        }else{
                            printf("Calling LIST FILEINFO\n");
                            return_error = list_fileinfo(char_arg, head_path_arg);
                            if(return_error < 0){
                                errorToString(return_error, "LIST FILEINFO");
                            }
                            error = 0;
                        }
                    }
                    else if (!strcmp(init_arg, "DIRECTORY\n")) {
                        printf("LIST DIRECTORY called\n");
                        listDirectory();
                        error = 0;
                    }
                    else if (!strcmp(init_arg, "BUFFERS\n")) {
                        printf("LIST BUFFER called\n");
                        listBuffers();
                        error = 0;
                    }
                    else{
                        error = 1;
                    }
                }
                if(error == 1){
                    textcolor(BRIGHT, RED, BLACK);
                    printf("Usage: LIST [DEVICES|FILEINFO <filename>|DIRECTORY|BUFFERS]\n");
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
                printf("INIT_FS <device>\n");
                printf("FORMAT <device> <fs_name> <blocksize>\n");
                printf("MOUNT <fs_name>\n");
                printf("OPEN <filename> [ NEW | READ-ONLY | READ-WRITE ]\n");
                printf("MKDIR <filepath>\n");
                printf("READ <filehandle> <block_number> <buf_ptr>\n");
                printf("WRITE <filehandle> <block_number> <buf_prt>    \n");
                printf("CLOSE <filehandle>\n");
                printf("DELETE <filepath>\n");
                printf("LIST [DEVICES|FILEINFO <filename>|DIRECTORY|BUFFERS]\n");
                printf("BUF_FLUSH <buf_prt>\n");
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
