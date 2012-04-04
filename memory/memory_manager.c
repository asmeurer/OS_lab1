/**
 * Memory OS Manager
 */

#include "memory_manager.h"

/* Inits all memory manager data structures, this includes the page table, the
 * physical memory array for LRU look up and finally the array to keep track of the
 * backing store */

void init_mem(){

	int i,j;
	/* Setting the LRU counter to 0, which will be updated by hardware */
	global_LRU_counter = 0;

	/* Setting each index of the table of page tables  to 0 to init it */

	for(i = 0;i < MAX_PROCESSES; i++){
		for(j = 0; j < MAX_PAGES_PER_PROCESS; j++){
		page_tables[i][j].phy_add = 0;
		page_tables[i][j].back_addr = 0;
		page_tables[i][j].bits = 0;
		}
	}
	
	for(i = 0; i < USER_PHY_MEM_NUM_FRAMES; i++){
		phy_mem[i].LRU = 0;
		phy_mem[i].mapped = 0;
	}

	for(i = 0;i <  sizeof(backing_store_free); i++){
		backing_store_free[i] = 0;
	}
}

/* This funtion looks through the page table looking at the first valid bit of each page entry
 * until it finds one then. Once a free slot in the array is found it sets the valid bit for each page
 * requested. 
 */
int alloc_pt (int num_pages){

	int page_table_id, phy_address, i, error;

	for(i = 0; i < MAX_PROCESSES; i++){

		if(page_tables[i][0].bits & P_BITMASK){
			page_table_id = i;
			break;
		}else{
			error = -7;
			return error;
		}	
	}

	for(i = 0; i< num_pages; i++){
		page_tables[page_tables_id][i] =  page_tables[page_table_id][i].bits | P_BITMASK;
	}

	
}



/* This function is to find a free frame in physical memory */

byte lru_lookup(){
	int i; 

	for (i = 0; i < USER_PHY_MEM_NUM_FRAMES; i++){
		if(phys_mem[i].LRU == 0){
			return i;
		}
	}
}

/* Find a free memory slot in the backing store.  This will correspond to a 0
 * bit in the backing_store_free byte array. */
short find_empty_back_addr() {
    int prefix;
    const int backing_store_free_size = BACK_STORE_SIZE/8;
    byte not_byte;
    int found = 0;
    int suffix = 0;

    for (prefix = 0; prefix < backing_store_free_size; prefix++) {
        if (~backing_store_free[prefix]) {
            /* A 0 bit corresponds to a free slot, so ! (logical not) will be
             * true if there is at least one 0 and false if it is all 1's. */
            found = 1;
            not_byte = ~backing_store_free[prefix];
            break;
        }
    }

    /* We got to the end of the loop and didn't find any free slots. */
    if (!found) {
        return NO_FREE_MEMORY;
    }

    /* We have the byte that has a 0 in it, now find where the first 0 is.
     * To check this, we use the fact that an unsigned byte has a 1 in the first bit if
     * and only if it is greater than or equal to pow(2, 8) = 256. */
    for (suffix = 0; suffix < 8; suffix++){
        if (not_byte >= 256) {
            break;
        }
        not_byte <<= 1;
    }

    /* Finally, convert the prefix and suffix into an address. */
    return (prefix << 3) + suffix;
}

/* Set the address addr to empty or full.  Returns errors if the memory is
 * already in the state it is being set to, or if the address is out of
 * bounds.  */
int set_back_addr_empty(short addr) {
    int prefix;
    int suffix;
    byte suffix_bitmask;

    if (addr > BACK_STORE_NUM_FRAME) {
        return ERROR_ADDR_OUT_OF_BOUNDS;
    }

    prefix = addr >> 3;
    suffix = addr & 0x7;
    /* Create the bitmask for the suffix */
    suffix_bitmask = 1 << suffix;

    if (~backing_store_free[prefix] & suffix_bitmask) {
        /* The memory was already set to empty */
        return ERROR_BACKING_EMPTY;
    } else {
        backing_store_free[prefix] = ~(~backing_store_free[prefix] | suffix_bitmask);
        return ERROR_SUCCESS;
    }
}

int set_back_addr_full(short addr) {
    int prefix;
    int suffix;
    byte suffix_bitmask;

    if (addr > BACK_STORE_NUM_FRAME) {
        return ERROR_ADDR_OUT_OF_BOUNDS;
    }

    prefix = addr >> 3;
    suffix = addr & 0x7;
    /* Create the bitmask for the suffix */
    suffix_bitmask = 1 << suffix;

    if (backing_store_free[prefix] & suffix_bitmask) {
        /* The memory was already set to full */
        return ERROR_BACKING_FULL;
    } else {
        backing_store_free[prefix] = backing_store_free[prefix] | suffix_bitmask;
        return ERROR_SUCCESS;
    }
}
//Remove this after
#include <stdio.h>

int page_fault (int page_table_index, int page_num){
	//TODO: Check to make sure page_table_index and page_num exist
	//Check first page number init
	//Will return 0 if 4th bit in bits not set
	if(page_tables[page_table_index, 0].bits & P_BITMASK){
		return ERROR_PAGE_TABLE_NOT_INIT;
	}
	//Check if page number init
	if(page_tables[page_table_index, page_num].bits & P_BITMASK){
		return ERROR_PAGE_NOT_INIT;
	}
	//TODO: figure out what error codes lru_lookup will return
	//Check for errors in lru_lookup
	int lru_index = lru_lookup();
	//If something in memory slot
	phy_mem_frame victimized = phy_mem[lru_index].mapped;
	if (victimized != null){
		//If victimized page is not in backing store
		if(victimized.bits & BMV_BITMASK){
			//Put into backing store, make refrence to it
			victimized.back_addr = find_empty_back_addr();
			if(set_back_addr_full(victimized.back_addr == ERROR_BACKING_FULL){
				return ERROR_BACKING_FULL;
			}
			//Set backing memory valid bit
			victimized.bits = victimized.bits | BMV_BITMASK;
		}else{
			//If victimized page is in backing store, check dirty bit
			//Dirty bit not set
			if(victimized.bits & D_BITMASK){
			}
			//Dirty bit set
			else{
				overwrite_backing_store(victimized.backing_store);
			}
		}
		
		
	}
	//
	
	
	
}

/*
int alloc_pt (int num_pages);
int dealloc_pt (int page_table_index);
int page_fault (int page_table_index, int page_num);
//Returns either index of phy_mem or error code

 */
/*
typedef struct{
    byte phy_addr;
    short back_addr;
    byte bits;
}page_table_entry;

typedef struct{
    unsigned int LRU;
    page_table_entry* mapped;
}phy_mem_frame;

//Page table allocated space for OS
page_table_entry page_tables[MAX_PROCESSES][MAX_PAGES_PER_PROCESS];

//Phy memory bookkeeping
phy_mem_frame phy_mem[USER_PHY_MEM_NUM_FRAMES];

//A byte array, where each bit in the byte represents 8 frames in the backing store
byte backing_store_free[BACK_STORE_NUM_FRAME/8];
*/
