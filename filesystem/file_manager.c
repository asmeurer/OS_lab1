/**
 * File Manager
 *
 * Group 2
 * Aaron Meurer
 * Oran Wallace
 * Sheng Lundquist
 */
#include "file_manager.h"

int init_fs (){
	return ERROR_SUCCESS;
}

int format(int device_num, char fs_name, int blocksize){
	int i;
	/*Check for correct num of devices*/
	if(device_num < 0 || device_num >= MAX_DEVICE){
		return ERROR_INVALID_DEVICE_NUM;
	}
	device *format_me = &device_array[device_num];
	/*Must be unmounted*/
	if(format_me->bits & DEVICE_MOUNTED_BITMASK){
		return ERROR_DEVICE_MOUNTED;
	}
	format_me->fs_name = fs_name;
	format_me->numblock = MEM_SIZE/blocksize;
	/*Erase*/
	for(i = 0; i < MEM_SIZE / 32; i++){
		format_me->bitmap[i] = 0;
	}
	format_me->bits = format_me->bits | DEVICE_FORMAT_BITMASK;
	return format_me->numblock;
}

int unmount(char fs_name){
	return ERROR_SUCCESS;
}

int add_to_open_table(){
	return ERROR_SUCCESS;
}
int open(char* filename, int option){
	switch(option){
		case OPEN_NEW:
			
			break;
		case OPEN_R:
			break;
		case OPEN_RW:
			break;
	}
}

int read(int filehandle, short block_number, int buf_ptr){
	block* temp;
	int error = 1;
	int i = 0;
	/*Check if file is open*/
	if (open_files[filehandle].open == 0){
		return ERROR_FILE_NOT_OPEN;
	}
	
	open_files[filehandle].file->blocktail = temp;
	/*Check if block number is part of file*/
	while (temp != null){
		if (temp->addr == block_number){
			error = 0;
		}
		temp = temp->next;
	}
	if (error == 1){
		return ERROR_BLOCK_NOT_IN_FILE;
	}
	/*Check buffer pointer*/
	if(buf_ptr < 0 || buf_ptr >= NUM_BUFFERS){
		return ERROR_BUFFER_NOT_EXIST;
	}
	/*Find next buffer slot*/
	for(i = 0; i < BUFFER_SIZE; i++){
		if(buffers[buf_ptr]->init == 0){
			buffers[buf_ptr]->init = 1;
			buffers[buf_ptr]->addr = block_number;
			buffers[buf_ptr]->access_type = READ;
			break;
		}
		if (i == BUFFER_SIZE){
			return ERROR_BUFFER_FULL;
		}
	}
	return ERROR_SUCCESS;
}
