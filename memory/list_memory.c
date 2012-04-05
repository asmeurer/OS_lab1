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
		printf("\nPage %4d:	Physical: ", i);
		//Physical memory valid bit set
		if (page_tables[page_table_id][i].bits & PMV_BITMASK){
			printf("0x%02X", page_tables[page_table_id][i].phy_addr);
		}
		//Physical memory valid bit not set
		else{
			printf("0x--");
		}
		printf("	Backing: ");
		//Backing memory valid bit set
		if (page_tables[page_table_id][i].bits & BMV_BITMASK){
			printf("0x%04X", page_tables[page_table_id][i].back_addr);
		}
		//Backing memory valid bit not set
		else{
			printf("0x----");
		}
		i++;
		if(i == MAX_PAGES_PER_PROCESS){
			break;
		}
	}
	printf("\n");
	return ERROR_SUCCESS;
}

void list_phy_mem(){
	int i, j, k;
	printf("\n********************************************************\n");
	printf("Physical Memory");
	printf("\n********************************************************");
	for(i = 0; i < USER_PHY_MEM_NUM_FRAMES; i++){
		printf("\nFrame number: %03d	", i);
		printf("LRU Stamp: 0x%08X		", phy_mem[i].LRU);
		printf("PTID/Page#: ");
		if(phy_mem[i].mapped == null){
			printf("Not Exist");
		}
		else{
			//Iterate processes
			for(j = 0; j < MAX_PROCESSES; j++){
				//Iterate pages
				for(k = 0; k < MAX_PAGES_PER_PROCESS; k++){
					//If page not set, break to next process
					if(!(page_tables[j][k].bits & P_BITMASK)){
						break;
					}
					//Check if pointers match
					if(&page_tables[j][k] == phy_mem[i].mapped){
						printf("%2d / %4d", j, k);
					}
				}
			}
		}
	}
	printf("\n");
}

void list_backing_store() {
    int i;
    char str[sizeof(byte)*8 + 1];
    const int backing_store_free_size = BACK_STORE_NUM_FRAME/8;
    const int num_cols = 128;   /* Should be a multiple of 8.  128 will
                                 * give a perfect square. */
    const int row_division = 5; /* The number of rows to include before a row
                                 * divider. */
    print_row_separator(row_division, num_cols);
    printf("\n");
    printf("|     |");
    for (i = 0; i < num_cols; i++) {
        if (i && !((i + 1) % 8)) {
            printf("%-8d|", i - 7);
        }
    }

    for (i = 0; i < backing_store_free_size; i++) {
        if (!(i % (num_cols/8))) {
            if (!(i % row_division * num_cols/8)) {
                printf("\n");
                print_row_separator(row_division, num_cols);
            }
            printf("\n");
            printf("|%5d|", i*8);
        }
        itodots(backing_store_free[i], str);
        print_colored_dots(str);
        printf("|");
    }
    printf("\n");
    print_row_separator(row_division, num_cols);
    printf("\n");
}

void list_physical_memory() {
    return;
}

/* Convert a number n into dots, where . is a 0 and * is a 1 from the binary
 * representation of n. */
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

/* Print dots from the itodots in color */
void print_colored_dots(char *str) {
    int i;
    const int bytes_in_byte = 8*sizeof(byte);

    for (i = 0; i < bytes_in_byte; i++) {
        if (str[i] == '*') {
            textcolor(BRIGHT, WHITE, BLACK);
            printf("%c", str[i]);
            textcolor(RESET, -1, -1);
        } else if (str[i] == '.') {
            textcolor(BRIGHT, RED, BLACK);
            printf("%c", str[i]);
            textcolor(RESET, -1, -1);
        } else {
            printf("%c", str[i]);
        }
    }
}

void print_row_separator(int row_division, int num_cols) {
    int j;
    int k;

    printf("+");

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
