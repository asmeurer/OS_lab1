/**
 * Memory OS Manager
 */

#include "memory_manager.h"
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
				overwrite_backing_store(victimized.back_addr);
			}
		}
	}
	
	
	
	
	
}

int overwrite_backing_store(short back_addr){
	/*Stub to write to backing store in case of dirty bit with victimize*/
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
