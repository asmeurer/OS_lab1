/**
 * File Manager
 *
 * Group 2
 * Aaron Meurer
 * Oran Wallace
 * Sheng Lundquist
 */

init_fs (char device){
	int i;
	if(i > 5){
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

