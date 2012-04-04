/**
 * Memory Test Interface
 *
 *	Group 2
 *	Aaron Meurer
 *	Oran Wallace
 *	Sheng Lundquist
 */

#include <stdio.h>
#include "memory_test.c"


int main(int argc, char *argv[]) {
    int error = 0;
    char command[20];
    int dest;
    int source;
    char message[MESSAGE_SIZE];
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
        printf("The Group 2 IPC Test runner!\n");
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
}
