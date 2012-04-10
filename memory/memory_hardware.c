/**
 * Memory Hardware Emulation

 Group 2
 Aaron Meurer
 Oran Wallace
 Sheng Lundquist

 */

#include "memory_hardware.h"
int HW_page_hit(int page_table_index, int page_num){
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
    if(!(page_tables[page_table_index][page_num].bits & PMV_BITMASK)){
        return ERROR_HARDWARE_NOT_IN_PHY_MEM;
    }
    byte addr = page_tables[page_table_index][page_num].phy_addr;
    global_LRU_counter++;
    checkoverflow();
    phy_mem[addr].LRU = global_LRU_counter;
    return ERROR_SUCCESS;
}
