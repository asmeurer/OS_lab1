/**
 * Memory hardware emulation header
 */

//Emulation of a page hit in hardware
//Will increment global counter
void HW_page_hit();

//Emulation of page fault in harddware
//Will throw interupt to call page fault function in memory manager
void HW_page_fault();
