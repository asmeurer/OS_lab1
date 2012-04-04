/**
 * Memory hardware emulation header
 */

#ifndef MEM_HW_H
#define MEM_HW_H
#include "memory_definitions.h"

//Emulation of a page hit in hardware
//Will increment global counter
int HW_page_hit(int page_table_index, int page_num);

#endif
