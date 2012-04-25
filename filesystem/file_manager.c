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
init_fs (char device){
	int i;
	if(i >= 5){
		return ERROR_DEVICE_NOT_KNOWN;
	}
	device_array[device].devicename = device;
	return ERROR_SUCCESS;
}

/* Mount: checks if the device has been inited and formated */
mount (char fsname){
	int i, error;
	for(i = 0; i < device_array.length; i++){
		if( device[i].fsname == fsname && (!(device[i].bits & DEVICE_FORMAT_BITMASK)) ){
			// Do something?
		return ERROR_SUCCES;
		}		
	}
	return ERROR_NOT_INITIALIZED_OR_FORMATED; 	
}

<<<<<<< HEAD
=======
int format(int device_num, char fs_name, int blocksize){
	int i;
	/*Check for correct num of devices*/
	if(device_num < 0 || device_num >= MAX_DEVICE){
		return ERROR_INVALID_DEVICE_NUM;
	}
	return ERROR_NOT_INITIALIZED_OR_FORMATED; 	
}
>>>>>>> 54282ffa809dbe1e18bf6c007fe539931cfab62c

