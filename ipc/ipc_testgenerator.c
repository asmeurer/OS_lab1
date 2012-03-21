/* Test file generator!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist

   This file is used to generate the file “tests” which is used by
   “scheduler_test.c” to perform various functions of our “processmanager.c”
   program.

*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"ipc_definitions.h"

int main() {
    int input;
    FILE *output = NULL;
    char *mode = "a";
    char outputFilename[] = "ipc/ipc_tests";
    char string[256];
    int source;
    int destination;
    int number;

    output = fopen(outputFilename, mode);

    if(output == NULL){
        printf("Cannot get output file.\n");

        exit(1);
    }

    printf("0) Add comment\n");
    printf("1) Init\n");
    printf("2) List\n");
    printf("3) Has Message\n");
    printf("4) Send\n");
    printf("5) Retrieve\n");
    printf("6) Quit\n");


    while(input != 6) {

        printf("Enter an option: ");
        scanf("%d", &input);

        switch(input) {
        case 0:
            printf("What comment would you like to add? ");
            scanf("\n%[^\n]", string);
            fprintf(output, "# %s\n", string);

            break;
        case 1:
            fprintf(output, "INIT_IPC");
            while (1) {
                printf("Which message managers would you like to initialize (0-9, End)? ");
                scanf("\n%c", string);
                if (string[0] == 'e' || string[0] == 'E') {
                    break;
                }
                fprintf(output, " %d", atoi(string));
            }
            fprintf(output, "\n");

            break;

        case 2:
            printf("What message manager would you like to list? ");
            scanf("\n%d", &number);
            fprintf(output, "LIST %d\n", number);
            break;

        case 3:
            printf("What message manager would you like to check? ");
            scanf("\n%d", &number);
            fprintf(output, "HAS_MESSAGE %d\n", number);

            break;
        case 4:
            printf("What should the source manager be? ");
            scanf("\n%d", &source);
            printf("What should the destination manager be? ");
            scanf("\n%d", &destination);
            printf("What should the message be? ");
            scanf("\n%[^\n]s", &string);
            fprintf(output, "SEND %d %d %s\n", source, destination, string);

            break;
        case 5:
            printf("What message manager should it retrieve the message from? ");
            scanf("\n%d", &number);
            fprintf(output, "Retrieve %d \n", number);

            break;
        default:

            if(input == 6){
                printf("Goodbye.\n");
            } else {
                printf("Invalid option.\n");
            }
            break;
        }
    }


    fclose(output);

    return 0;

}
