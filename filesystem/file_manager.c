/**
 * File Manager
 *
 * Group 2
 * Aaron Meurer
 * Oran Wallace
 * Sheng Lundquist
 */
#include "file_manager.h"

/* Init_fs: checks if the device is known, if so set the device name */
int init_fs (int device){
	int i;
	/*Unmount all devices*/
	for (i = 0; i < MAX_DEVICE; i++){
		device_array[i].bits = device_array[i].bits & (~DEVICE_MOUNTED_BITMASK);
	}
	/*Clear open file array*/
	for (i = 0; i < MAX_OPEN; i++){
		open_files[i].open = 0;
		open_files[i].file = null;
	}
	return ERROR_SUCCESS;
}


/* Mount: checks if the device has been inited and formated, if so mounts it. Otherwise, returns an error. */
int mount (char fsname){
	int i;
	for(i = 0; i < MAX_DEVICE; i++){
		if(device_array[i].fsname == fsname && (!(device_array[i].bits & DEVICE_FORMAT_BITMASK))){
			device_array[i].bits | DEVICE_MOUNTED_BITMASK;
			return ERROR_SUCCESS;
		}		
	}
	
	return ERROR_NOT_INITIALIZED_OR_FORMATED; 	
}

int format(int device_num, char fs_name, int blocksize){
	/*Check for correct num of devices*/
	int i;
	
	if(blocksize != 4 || blocksize != 8 || blocksize != 16){
		return ERROR_INVALID_BLOCK_SIZE;
	}
	
	for(i = 0; i < MAX_DEVICE; i++){
		if(!(device_array[device_num].bits & DEVICE_MOUNTED_BITMASK)){
			device_array[device_num].bits | DEVICE_FORMAT_BITMASK
			device_array[device_num].fs_name = fs_name;
		}else if(device_array[device_num].bits & DEVICE_MOUNTED_BITMASK){
			return ERROR_DEVICE_MOUNTED;		
		}
	}
	
	if(device_num < 0 || device_num >= MAX_DEVICE){
		return ERROR_INVALID_DEVICE_NUM;
	}
	
	//TODO : change bit map to zero, traverse linked list of FCBs and free every block
	return ERROR_SUCCESS;
}

