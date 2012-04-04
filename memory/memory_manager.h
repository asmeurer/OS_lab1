/**
 * Memory OS Manager Header
 */

#ifndef MEM_MAN_H
#define MEM_MAN_H
#include "memory_definitions.h"
//Initialize OS_page_table
void init_mem();

int alloc_pt (int num_pages);
int dealloc_pt (int page_table_index);
int page_fault (int page_table_index, int page_num);
//Returns either index of phy_mem or error code
byte lru_lookup();
short find_empty_back_addr();
int set_back_addr_empty(short addr);
int set_back_addr_full(short addr);

#endif
