/**
 * File Manager
 *
 * Group 2
 * Aaron Meurer
 * Oran Wallace
 * Sheng Lundquist
 */



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

