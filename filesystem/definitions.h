/**
 * Definitions header
 *
 * Group 2
 * Aaron Meurer
 * Oran Wallace
 * Sheng Lundquist
 */

/**
 * Memory Definitions Header
 */

#ifndef MEM_DEF_H
#define MEM_DEF_H

typedef unsigned char byte;

/* Error Codes */
#define ERROR_SUCCESS 0
#define ERROR_INVALID_DEVICE_NUM -1

/*Constants*/
#define BUFFER_SIZE 5
#define MAX_DEVICE 5
#define MAX_OPEN 8
#define FCB_DIR_BITMASK 0x01
#define FCB_WRITE_ACC_BITMASK 0x02
#define DEVICE_FORMAT_BITMASK 0x01
#define DEVICE_MOUNTED_BITMASK 0x02
#define NAME_LIMIT 11
/*256 MB*/
#define MEM_SIZE 262144

/*Buffers*/
short rBuffers [MAX_DEVICE][BUFFER_SIZE];
short wBuffers [MAX_DEVICE][BUFFER_SIZE];
byte rBufSize [MAX_DEVICE];
byte wBufSize [MAX_DEVICE];

/*FCB*/
typedef struct{
	char filename[NAME_LIMIT];
	/*0 0 0 0 0 0 (Write access) (Directory)*/
	byte bits;
	struct fcb* dirHead;
	short* blockhead;
	short* blocktail;
	struct fcb* next;
	struct fcb* prev;
	byte device_num;
}fcb;

/*Device*/
typedef struct{
	fcb* filehead;
	/*Memory size divided by smallest allowed block, divided by 8 bits per byte*/
	byte bitmap[MEM_SIZE / 32];
	byte blocksize;
	char fs_name;
	/*0 0 0 0 0 0 (Mounted) (Formated)*/
	byte bits;
}device;

/*Device list*/
device device_array [MAX_DEVICE];

/*Open files array*/
fcb open_files[MAX_OPEN];

#endif
