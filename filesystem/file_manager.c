/**
 * File Manager
 *
 * Group 2
 * Aaron Meurer
 * Oran Wallace
 * Sheng Lundquist
 */

int init_fs (){
}

int format(int device_num, char fs_name, int blocksize){
	if(device_num < 0 || device_num >= MAX_DEVICE){
		return ERROR_INVALID_DEVICE_NUM;
	}
	device format_me= device_array[device_num];
	format_me.fs_name = fs_name;
	format_me.blocksize = blocksize;
	
	
}
