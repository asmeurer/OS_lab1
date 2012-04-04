/**
 * Memory Definitions Header
 */

#ifndef MEM_DEF_H
#define MEM_DEF_H

//Error Codes
#define ERROR_SUCCESS 0
#define ERROR_PAGE_TABLE_NOT_INIT -1
#define ERROR_PAGE_NOT_INIT -2
#define ERROR_NO_FREE_MEMORY -3
#define ERROR_ADDR_OUT_OF_BOUNDS -4
#define ERROR_BACKING_EMPTY -5
#define ERROR_BACKING_FULL -6


#define null = 0;

//Backing store memory in KB
#define BACK_STORE_SIZE (64 * 1024)
//Physical memory in KB
#define PHY_MEM_SIZE 1024
//Backing store num frames
#define BACK_STORE_NUM_FRAME (BACK_STORE_SIZE / PAGE_SIZE)
//Physical num frames
#define PHY_MEM_NUM_FRAME (PHY_MEM_SIZE / PAGE_SIZE)
//Page/Frame size in KB
#define PAGE_SIZE 4

//OS and User physical memory in KB
#define OS_PHY_MEM_SIZE (OS_PHY_MEM_NUM_FRAMES * PAGE_SIZE)
#define USER_PHY_MEM_SIZE (PHY_MEM_SIZE - OS_PHY_MEM_SIZE)

//OS and User physical memory number frames
#define OS_PHY_MEM_NUM_FRAMES 20
#define USER_PHY_MEM_NUM_FRAMES (USER_PHY_MEM_SIZE / 4)

//Max number of processes
#define MAX_PROCESSES 16
//Max number of pages per process
#define MAX_PAGES_PER_PROCESS 1024

//Phy Valid Bit Bitmask
#define PMV_BITMASK 0x01
//Backing Valid Bit Bitmask
#define BMV_BITMASK 0x02
//Dirty Bit Bitmask
#define D_BITMASK 0x04
//Page Valid Bitmask
#define P_BITMASK 0x08

typedef unsigned char byte;

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

//Global counter for LRU that hardware will update
unsigned int global_LRU_counter;

#endif
