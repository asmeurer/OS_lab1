/* Functions to list out the memory */

#include <stdio.h>
#include "memory_definitions.h"
#include "list_memory.h"
int list_page_table(int page_table_id){
	int i = 0;
	//Check if page table is init
	if(!(page_tables[page_table_id][0].bits & P_BITMASK)){
		return ERROR_PAGE_TABLE_NOT_INIT;
	}
	printf("\n********************************************************\n");
	printf("Page Table %d", page_table_id);
	printf("\n********************************************************");
	//Loop until page not init
	while(page_tables[page_table_id][i].bits & P_BITMASK){
		printf("\nPage %d:	Physical: ", i);
		//Physical memory valid bit set
		if (page_tables[page_table_id][i].bits & PMV_BITMASK){
			printf("%x", page_tables[page_table_id][i].phy_addr);
		}
		//Physical memory valid bit not set
		else{
			printf("Not In Memory");
		}
		printf("	Backing: ");
		//Backing memory valid bit set
		if (page_tables[page_table_id][i].bits & BMV_BITMASK){
			printfg("%x", page_tables[page_table_id][i].back_addr);
		}
		//Backing memory valid bit not set
		else{
			printf("Not In Memory");
		}	
		i++;
	}
	printf("\n");
	return ERROR_SUCCESS;
}


void list_backing_store() {
    int i;
    int j;
    int k;
    char str[sizeof(byte)*8 + 1];
    const int backing_store_free_size = BACK_STORE_NUM_FRAME/8;
    const int num_cols = 128;   /* Should be a multiple of 8.  128 will
                                       * give a perfect square. */

    printf("     |");
    for (i = 0; i < num_cols; i++) {
        if (i && !((i + 1) % 8)) {
            printf("%-8d|", i - 7);
        }
    }

    for (i = 0; i < backing_store_free_size; i++) {
        if (!(i % (num_cols/8))) {
            if (!(i % 5 * num_cols/8)) {
                printf("\n");
                for (j = 0; j < 5; j++) {
                    printf("-");
                }
                for (k = 0; k < num_cols/8; k++) {
                    printf("+");
                    for (j = 0; j < 8; j++) {
                        printf("-");
                    }
                }
                printf("+");
            }
            printf("\n");
            printf("%5d|", i*8);
        }
        itodots(backing_store_free[i], str);
        printf("%s|", str);
    }
    printf("\n");
}

void itodots(int n, char *str) {
    int place = 0;
    int i;
    const int bytes_in_byte = 8*sizeof(byte);

    for (i = 0; i < bytes_in_byte; i++){
        if (n & 1){
            str[bytes_in_byte - 1 - place] = '*';
        } else {
            str[bytes_in_byte - 1 - place] = '.';
        }
        n >>= 1;
        place++;
    }
    str[bytes_in_byte] = '\0';
}
