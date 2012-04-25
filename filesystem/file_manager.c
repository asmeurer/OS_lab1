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

/* Mount: checks if the device has been inited and formated */
int mount (char fsname){

	int i, error;
	for(i = 0; i < MAX_DEVICE; i++){
		if( device[i].fsname == fsname && (!(device[i].bits & DEVICE_FORMAT_BITMASK)) ){
			device[i].buts | DEVICE_MOUNTED_BITMASK;
		return ERROR_SUCCESS;
		}		
	}
	return ERROR_NOT_INITIALIZED_OR_FORMATED; 	
	* */
	return ERROR_SUCCESS;
}

int format(int device_num, char fs_name, int blocksize){
	/*Check for correct num of devices*/
	if(device_num < 0 || device_num >= MAX_DEVICE){
		return ERROR_INVALID_DEVICE_NUM;
	}
	return ERROR_NOT_INITIALIZED_OR_FORMATED;
}

