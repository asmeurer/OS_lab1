#ifndef FILE_HARD
#define FILE_HARD

#include <stdlib.h>
#include "definitions.h"

void Free(void *ptr);
fcb *malloc_file();
block *malloc_block();
struct block_queue_t *malloc_block_queue();
struct dir_queue_t *malloc_dir_queue();

#endif
