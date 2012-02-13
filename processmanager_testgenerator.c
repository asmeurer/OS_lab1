/* Test file generator!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist

   This file is used to generate the file “tests” which is used by
   “queuemanager_test.c” to perform various functions of our “queuemanager.c”
   program.

*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main() {

    int i = 0;
    FILE *output = NULL;
    char *mode = "w";
    char outputFilename[] = "processmanager_tests";
    int input=0, number=0, pid=0;
    int psw=0, page=0, reg1=0, reg2=0, reg3=0;
    char string[256];

    output = fopen(outputFilename, mode);

    if(output == NULL){
        printf("Cannot get output file.\n");

        exit(1);
    }

    printf("0) Add comment\n");
    printf("1) Create\n");
    printf("2) List\n");
    printf("3) Go\n");
    printf("4) Unwait\n");
    printf("5) EOQuantum\n");
    printf("6) EOLife\n");
    printf("7) Wait\n");
    printf("8) Quit\n");


    while(input != 8) {

        printf("Enter an option: ");
        scanf("%d", &input);

        switch(input){
        case 0:
            printf("What comment would you like to add? ");
            scanf("\n%[^\n]", string);
            fprintf(output, "# %s\n", string);

            break;
        case 1:
            printf("How many would you like to create? ");
            scanf("%d", &number);

            for(i = 0; i < number; i++){
                psw = random() % 50;
                page = random() % 25;
                reg1 = random() % 20;
                reg2 = random() % 20;
                reg3 = random() % 20;

                pid++;

                fprintf(output, "CREATE %d %d %d %d %d %d\n", pid, psw, page, reg1, reg2, reg3);
            }
            number = 0;
            i = 0;
            break;
        case 2:

            printf("What queue would you like to list (new|terminated|ready|running|waiting|all|sched)? ");
            scanf("%s", string);

            fprintf(output, "LIST %s\n", string);
            break;
        case 3:

            fprintf(output, "GO\n");

            break;
        case 4:

            printf("What pid should be unwaited? ");
            scanf("%d", &number);
            fprintf(output, "UNWAIT %d\n", number);

            break;
        case 5:

            fprintf(output, "EOQUANTUM\n");

            break;
        case 6:

            fprintf(output, "EOLIFE\n");

            break;
        case 7:

            fprintf(output, "WAIT\n");

            break;
        default:

            if(input == 8){
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
