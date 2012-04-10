/* Test file generator!

   Group 2
   Aaron Meurer
   Oran Wallace
   Sheng Lundquist

   This file is used to generate the file a tests which is used by memory.test. 

*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


/**
 * The main function to the test generator
 * @return Returns 0
 */
int main() {
    int input;
    FILE *output = NULL;
    char *mode = "a";
    char outputFilename[] = "mem_tests";
    char string[256];
    int pagenum;
    int number;
    int id;

    output = fopen(outputFilename, mode);

    if(output == NULL){
        printf("Cannot get output file.\n");

        exit(1);
    }

    printf("0) Add comment\n");
    printf("1) Init\n");
    printf("2) List\n");
    printf("3) Alloc pt\n");
    printf("4) Dealloc pt\n");
    printf("5) Page fault\n");
    printf("6) Page hit\n");
    printf("7) Fill physical memory.\n"); 
    printf("8) Quit\n");


    while(input != 8) {

        printf("Enter an option: ");
        scanf("%d", &input);

        switch(input) {
        case 0:
            printf("What comment would you like to add? ");
            scanf("\n%[^\n]", string);
            fprintf(output, "# %s\n", string);

            break;
        case 1:
            fprintf(output, "INIT_MEM\n");
            break;

        case 2:
            printf("What would you like to list? \n");
	    printf(" 1) User.\n");
	    printf(" 2) System.\n");
	    printf(" 3) Page Table.\n");
            scanf("\n%d", &number);
	    if(number == 1 ){
		fprintf(output, "LIST USER\n");
	    }else if(number == 2){
		fprintf(output, "LIST SYSTEM\n");
	    }else if(number == 3){
		printf("What is the page table ID? ");
		scanf("\n%d", &id);
		fprintf(output, "LIST PAGETABLE %d\n", id);
	    }
            break;

        case 3:
            printf("How many pages to allocate?");
            scanf("\n%d", &number);
            fprintf(output, "ALLOC_PT %d\n", number);

            break;
        case 4:
            printf("What is the ID of the page table to deallocate?");
            scanf("\n%d", &id);
            fprintf(output, "DEALLOC_PT %d\n", id);

            break;
        case 5:
            printf("What is the ID of the page table?");
            scanf("\n%d", &id);
	    printf("What is the page number?");
	    scanf("\n%d", &pagenum);
            fprintf(output, "PAGE_FAULT %d %d \n", id, pagenum );

            break;
	case 6:
		printf("What is the ID of the page table? ");
		scanf("\n%d", &id);
		printf("What is the page that is accesses? ");
		scanf("\n%d", &pagenum);
		fprintf(output, "PAGE_HIT %d %d\n", id, pagenum);
	    break;

	case 7:
	    	printf("What is the ID of the page table? ");
		scanf("\n%d", &id);
		fprintf(output, "FILL_PHY_MEM %d\n", id);
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
