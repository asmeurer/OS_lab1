/**
 * File manager header
 *
 * Group 2
 * Aaron Meurer
 * Oran Wallace
 * Sheng Lundquist
 */


#ifndef FILE_MAN_H
#define FILE_MAN_H

#include "definitions.h"
#include "file_hardware.h"

int init_fs(int device);
int format(int device_num, char fsname, int blocksize);
int mount(char fsname);
int create_file(char fsname, struct path file_path);
int create_dir(char fsname, struct path file_path);
int get_device(char fsname);
int filename_eq(char *string1, char *string2);

#endif
