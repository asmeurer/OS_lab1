/**
 * Memory OS Manager
 */

//Remove this after
#include <stdio.h>

#include "memory_manager.h"

void init_mem(){

    int i,j;
    global_LRU_counter = 0;

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

int alloc_pt (int num_pages){



}


/* Find a free memory slot in the backing store.  This will correspond to a 0
 * bit in the backing_store_free byte array. */
short find_empty_back_addr() {
    int prefix;
    const int backing_store_free_size = BACK_STORE_SIZE/8;
    byte not_byte;
    int found = 0;
    int suffix = 0;
    int shift_mask;

    for (prefix = 0; prefix < backing_store_free_size; prefix++) {
        if (backing_store_free[prefix] != 0b11111111) {
            found = 1;
            not_byte = ~backing_store_free[prefix];
            break;
        }
    }

    /* We got to the end of the loop and didn't find any free slots. */
    if (!found) {
        return ERROR_NO_FREE_MEMORY;
    }

    /* We have the byte that has a 0 in it, now find where the first 0 is.
     * To check this, we create a bitmask 0b1000000 and shift it right until
     * we find the first 1 bit. */
    shift_mask = 0b1000000;
    for (suffix = 0; suffix < 8; suffix++){
        if (not_byte & shift_mask) {
            break;
        }
        shift_mask >>= 1;
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

int page_fault (int page_table_index, int page_num){
	//TODO: Check to make sure page_table_index and page_num exist
	//Check first page number init
	//Will return 0 if 4th bit in bits not set
	if(!(page_tables[page_table_index][0].bits & P_BITMASK)){
		return ERROR_PAGE_TABLE_NOT_INIT;
	}
	//Check if page number init
	if(!(page_tables[page_table_index][page_num].bits & P_BITMASK)){
		return ERROR_PAGE_NOT_INIT;
	}
	//Check if that page already exists in phy memory
	if(page_tables[page_table_index][page_num].bits & PMV_BITMASK){
		return ERROR_HARDWARE_ALREADY_IN_PHY_MEM;
	}
	
	//TODO: figure out what error codes lru_lookup will return
	//Check for errors in lru_lookup
	byte lru_index = lru_lookup();
	
	
	phy_mem_frame *victimized = phy_mem[lru_index].mapped;
	
	//If something in memory slot
	if (victimized != null){
		//If victimized page is not in backing store
		if(!(victimized->bits & BMV_BITMASK)){
			//Put into backing store, make refrence to it
			victimized->back_addr = find_empty_back_addr();
			if(set_back_addr_full(victimized->back_addr == ERROR_BACKING_FULL){
				return ERROR_BACKING_FULL;
			}
			//Write to backing store
			write_backing_store();
			//Set backing memory valid bit
			victimized->bits = victimized->bits | BMV_BITMASK;
		}else{
			//If victimized page is in backing store, check dirty bit
			//Dirty bit is set
			if(victimized->bits & D_BITMASK){
				//Write to backing store
				write_backing_store();
			}
		}
		//unset phy mem valid bit for victimized
		victimized->bits = victimized->bits & ~PMV_BITMASK;
	}
	//Put new page into memory
	//Map phy_mem to that page table
	phy_mem[lru_index].mapped = &page_tables[page_table_index][page_num];
	//Update page table
	page_tables[page_table_index][page_num].phy_addr = lru_index;
	//Set phy mem valid bit
	page_tables[page_table_index][page_num].bit = page_tables[page_table_index][page_num].bit | PMV_BITMASK;
}

int write_backing_store(){
	/*Stub to write to backing store*/
}

/*
  int alloc_pt (int num_pages);
  int dealloc_pt (int page_table_index);
  int page_fault (int page_table_index, int page_num);
//Returns either index of phy_mem or error code
int lru_lookup();
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