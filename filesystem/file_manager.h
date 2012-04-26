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
#include "file_queuemanager.h"
#include "file_hardware.h"

int init_fs(int device);
int _format(int device_num, char fs_name, int blocksize);
int mount(char fs_name);
int unmount(char fs_name);
fcb *get_file(int dev, path *file_path);
int close(int filehandle);
int open(char fs_name, path *file_path, int write);
int write(int filehandle, unsigned int block_number, int buf_ptr);
int read(int filehandle, unsigned int block_number, int buf_ptr);
int create(char fs_name, struct path *file_path, int dir);
int delete(char fs_name, struct path *file_path);
int delete_internal(int dev, fcb *file);
int get_device(char fs_name);
int filename_eq(char *string1, char *string2);
void filename_copy(char *source, char *dest);
unsigned int find_empty_block(int dev);
int set_block_empty(int dev, unsigned int addr);
int set_block_full(int dev, unsigned int addr);

#endif
