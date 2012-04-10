/**
 * Memory OS Manager
 *
 * Group 2
 * Aaron Meurer
 * Oran Wallace
 * Sheng Lundquist
 */

#include "memory_manager.h"

/* Inits all memory manager data structures, this includes the page table, the
 * physical memory array for LRU look up and finally the array to keep track
 * of the backing store */

void init_mem(){

    int i,j;
    /* Setting the LRU counter to 0, which will be updated by hardware */
    global_LRU_counter = 0;

    /* Setting each index of the table of page tables  to 0 to init it */

    for(i = 0;i < MAX_PROCESSES; i++){
        for(j = 0; j < MAX_PAGES_PER_PROCESS; j++){
            page_tables[i][j].phy_addr = 0;
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

/**
 * This funtion looks through the page table looking at the first valid bit of
 * each page entry until it finds one then. Once a free slot in the array is
 * found it sets the valid bit for each page requested.
 */
int alloc_pt (int num_pages){

    int page_table_id=-1, i;

    /* Check to see if the amount of pages given is past the maximum amount
     * allowed for a process */
    if(num_pages > MAX_PAGES_PER_PROCESS){
        return ERROR_MAX_PAGE_LIMIT;
    }

    for(i = 0; i < MAX_PROCESSES; i++){
        if(!(page_tables[i][0].bits & P_BITMASK)){
            page_table_id = i;
            break;
        }
    }

    if(page_table_id == -1){
        /* If page_table_id is 0 then every row of the page table has been
         * examined and they are all full, thu the max process limit has been
         * reached */
        return ERROR_MAX_PROCESSES_REACHED;
    }

    for(i = 0; i< num_pages; i++){
        page_tables[page_table_id][i].bits = page_tables[page_table_id][i].bits | P_BITMASK;
    }

    return page_table_id;
}

/* This function deallocate a page table from its index */
int dealloc_pt (int page_table_index){

    int i;
    byte phy_mem_addr;
    /* Checks to see if the first entry is 0, if so the page table hasnt been
     * allocated */
    if(!(page_tables[page_table_index][0].bits & P_BITMASK)){
        return ERROR_PAGE_TABLE_NOT_INIT;
    }

    for(i = 0; i < MAX_PAGES_PER_PROCESS; i++){
        if(!(page_tables[page_table_index][i].bits & P_BITMASK)){
            break;
        }

        /* Check to see if the page is in physical memory */
        if(page_tables[page_table_index][i].bits & PMV_BITMASK){
            phy_mem_addr = page_tables[page_table_index][i].phy_addr;
            phy_mem[phy_mem_addr].LRU = 0;
            phy_mem[phy_mem_addr].mapped = null;
        }
        /*Check to see if the page is in backing store*/
        if(page_tables[page_table_index][i].bits & BMV_BITMASK) {
            set_back_addr_empty( page_tables[page_table_index][i].back_addr );
        }
        /* Resetting all values in the page table */
        page_tables[page_table_index][i].phy_addr = 0;
        page_tables[page_table_index][i].back_addr = 0;
        page_tables[page_table_index][i].bits = 0;
    }
    return ERROR_SUCCESS;
}


/* This function is to find a free frame in physical memory */
byte lru_lookup(){
    byte i;

    int smallest = phy_mem[0].LRU;
    int smallest_index = 0;

    for (i = 1; i < USER_PHY_MEM_NUM_FRAMES; i++){
        if(phy_mem[i].LRU < smallest ){
            smallest = phy_mem[i].LRU;
            smallest_index = i;

        }
    }
    return smallest_index;

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
        if (backing_store_free[prefix] != 0xff) {
            found = 1;
            not_byte = ~backing_store_free[prefix];
            break;
        }
    }

    /* We got to the end of the loop and didn't find any free slots. */
    if (!found) {
        return ERROR_NO_FREE_MEMORY;
    }

    /* We have the byte that has a 0 in it, now find where the first 0 is.  To
     * check this, we create a bitmask 0b1000000 and shift it right until we
     * find the first 1 bit. */
    shift_mask = 0x80;
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
    suffix_bitmask = 1 << (7 - suffix);

    if (~backing_store_free[prefix] & suffix_bitmask) {
        /* The memory was already set to empty */
        return ERROR_BACKING_EMPTY;
    } else {
        backing_store_free[prefix] = ~(~backing_store_free[prefix] | suffix_bitmask);
        return ERROR_SUCCESS;
    }
}

/* Same as set_back_addr_empty. */
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
    suffix_bitmask = 1 << (7 - suffix);

    if (backing_store_free[prefix] & suffix_bitmask) {
        /* The memory was already set to full */
        return ERROR_BACKING_FULL;
    } else {
        backing_store_free[prefix] = backing_store_free[prefix] | suffix_bitmask;
        return ERROR_SUCCESS;
    }
}

/* Note: the page table must have at least the USER_PHY_MEM_NUM_FRAMES allocated */
int fill_phy_mem(int page_table_id){
    int i;
    if(!(page_tables[page_table_id][0].bits & P_BITMASK)){
        return ERROR_PAGE_TABLE_NOT_INIT;
    }
    if(!(page_tables[page_table_id][USER_PHY_MEM_NUM_FRAMES-1].bits & P_BITMASK)){
        return ERROR_PT_NOT_ENOUGH_PAGES;
    }
    for (i = 0; i < USER_PHY_MEM_NUM_FRAMES; i++){
        page_fault(page_table_id, i);
    }
    return ERROR_SUCCESS;
}


int page_fault(int page_table_index, int page_num){
    /* Check first page number init.  Will return 0 if 4th bit in bits not
     * set */
    if(!(page_tables[page_table_index][0].bits & P_BITMASK)){
        return ERROR_PAGE_TABLE_NOT_INIT;
    }
    /* Check if page number init */
    if(!(page_tables[page_table_index][page_num].bits & P_BITMASK)){
        return ERROR_PAGE_NOT_INIT;
    }
    /* Check if that page already exists in phy memory */
    if(page_tables[page_table_index][page_num].bits & PMV_BITMASK){
        return ERROR_HARDWARE_ALREADY_IN_PHY_MEM;
    }

    /* Find lru frame */
    byte lru_index = lru_lookup();

    page_table_entry *victimized = phy_mem[lru_index].mapped;

    /* If something in memory slot */
    if (victimized != null){
        /* If victimized page is not in backing store */
        if(!(victimized->bits & BMV_BITMASK)){
            /* Put into backing store, make refrence to it */
            victimized->back_addr = find_empty_back_addr();
            if(set_back_addr_full(victimized->back_addr) == ERROR_BACKING_FULL) {
                return ERROR_BACKING_FULL;
            }
            /* Write to backing store */
            write_backing_store();
            /* Set backing memory valid bit */
            victimized->bits = victimized->bits | BMV_BITMASK;
        }else{
            /* If victimized page is in backing store, check dirty bit */
            /* Dirty bit is set */
            if(victimized->bits & D_BITMASK){
                /* Write to backing store */
                write_backing_store();
            }
        }
        /* unset phy mem valid bit for victimized */
        victimized->bits = victimized->bits & ~PMV_BITMASK;
    }
    /* Put new page into memory */
    /* Map phy_mem to that page table */
    phy_mem[lru_index].mapped = &page_tables[page_table_index][page_num];
    /* Time stamp the LRU count to that address */
    global_LRU_counter++;
    checkoverflow();
    phy_mem[lru_index].LRU = global_LRU_counter;
    /* Update page table */
    page_tables[page_table_index][page_num].phy_addr = lru_index;
    /* Set phy mem valid bit */
    page_tables[page_table_index][page_num].bits = page_tables[page_table_index][page_num].bits | PMV_BITMASK;

    return ERROR_SUCCESS;
}

void checkoverflow(){
    int i;
    /* If about to overflow, reset each frame in phy mem's LRU and the LRU
     * global counter */
    if (global_LRU_counter >= 0xFFFFFFFF){
        for(i = 0; i < PHY_MEM_NUM_FRAME; i++){
            phy_mem[i].LRU = 0;
        }
        global_LRU_counter = 1;
    }
}

int write_backing_store(){
    /*Stub to write to backing store */
    return 0;
}
