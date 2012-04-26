/**
 * Memory Test Interface Header
 */

#ifndef MEM_TEST_H
#define MEM_TEST_H

#include "file_manager.h"
#include "file_hardware.h"

#include "definitions.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "../shared/textcolor.h"
#include <string.h>
#include <ctype.h>
#include <limits.h>

void list_devices();
int list_fileinfo(char fs_name, path *file_path);
char *fgetstring(FILE* fFile);
path* parsePath(char* pathname, char* fs);
int strToIntArg(char* string);
void printPath(path* head, char fs_name);
void errorToString(int error, char* command);
void listRec(fcb* file, int count);
void listDirectory();
void print_tabs(int count);

#endif
