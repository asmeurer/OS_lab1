/* Functions to list the memory */

#ifndef MEM_LIST_H
#define MEM_LIST_H
#include "memory_definitions.h"

#include "../shared/textcolor.h"

void list_backing_store();
void itodots(int n, char *str);
void print_colored_dots(char *str);
int list_page_table(int page_table_id);
void print_row_separator(int row_division, int num_cols);
void list_phy_mem();

#endif
