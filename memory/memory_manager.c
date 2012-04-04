/**
 * Memory OS Manager
 */
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

