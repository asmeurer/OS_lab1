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

/* device[i].fsname == fsname && (!(device[i].bits & DEVICE_FORMAT_BITMASK)) ){
			device[i].bits | DEVICE_MOUNTED_BITMASK; */

/* Mount: checks if the device has been inited and formated, if so mounts it. Otherwise, returns an error. */
int mount (char fsname){
	int i, error;
	for(i = 0; i < MAX_DEVICE; i++){
		if(device_array[i].fsname == fsname && (!(device_array[i].bits & DEVICE_FORMAT_BITMASK))){
			device_array[i].bits | DEVICE_MOUNTED_BITMASK;
			return ERROR_SUCCESS;
		}
	}

	return ERROR_NOT_INITIALIZED_OR_FORMATED;
}

int format(int device_num, char fsname, int blocksize){
	/*Check for correct num of devices*/
	if(device_num < 0 || device_num >= MAX_DEVICE){
		return ERROR_INVALID_DEVICE_NUM;
	}
	return ERROR_NOT_INITIALIZED_OR_FORMATED;
}

int create_file(char fsname, struct path file_path)
{
    struct path *next = &file_path;

    while (
    return 0;
}

int create_dir(char fsname, struct path file_path)
{
    return 0;
}


/* Convert a filesystem character name to a device number */
int get_device(char fsname)
{
    int i;
    for (i=0; i < MAX_DEVICE; i++) {
        if (device_array[i].fsname == fsname) {
            return i;
        }
    }
    return ERROR_BAD_FSNAME;
}
