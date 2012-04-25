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
init_fs (int device){
	int i;
	if(i >= 5){
		return ERROR_DEVICE_NOT_KNOWN;
	}
	for(i = 0; i < device.length; i ++){
		
	}
	return ERROR_SUCCESS;
}

/* Mount: checks if the device has been inited and formated, if so mounts it. Otherwise, returns an error. */
mount (char fsname){
	int i, error;
	for(i = 0; i < MAX_DEVICE; i++){
		if( device[i].fsname == fsname && (!(device[i].bits & DEVICE_FORMAT_BITMASK)) ){
			device[i].buts | DEVICE_MOUNTED_BITMASK;
		return ERROR_SUCCESS;
		}		
	}
	return ERROR_NOT_INITIALIZED_OR_FORMATED;
}

int format(int device_num, char fs_name, int blocksize){
	int i;
	/*Check for correct num of devices*/
	if(device_num < 0 || device_num >= MAX_DEVICE){
		return ERROR_INVALID_DEVICE_NUM;
	}
	return ERROR_NOT_INITIALIZED_OR_FORMATED;
}

