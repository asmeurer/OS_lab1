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

#ifndef FILE_DEF_H
#define FILE_DEF_H

typedef unsigned char byte;

#define null 0

/* Error Codes */
#define ERROR_SUCCESS 0
#define ERROR_INVALID_DEVICE_NUM -1
#define ERROR_DEVICE_MOUNTED -2
#define ERROR_FILE_NOT_OPEN -3
#define ERROR_BLOCK_NOT_IN_FILE -4
#define ERROR_BUFFER_FULL -5
#define ERROR_BUFFER_NOT_EXIST -6
#define ERROR_NOT_INITIALIZED_OR_FORMATED -7
#define ERROR_DEVICE_NOT_KNOWN -8
#define ERROR_SOURCE_QUEUE_NOT_EXIST -9
#define ERROR_BAD_FILE_PTR -10
#define ERROR_ARG_NOT_INT -11

/*Constants*/
#define null 0
#define BUFFER_SIZE 5
#define MAX_DEVICE 5
#define MAX_OPEN 8
#define FCB_DIR_BITMASK 0x01
#define FCB_WRITE_ACC_BITMASK 0x02
#define DEVICE_FORMAT_BITMASK 0x01
#define DEVICE_MOUNTED_BITMASK 0x02
#define NAME_LIMIT 11
#define NUM_BUFFERS 5
/*256 MB*/
#define MEM_SIZE 262144

#define OPEN_NEW 1
#define OPEN_R 2
#define OPEN_RW 3
#define READ 1
#define WRITE 2

/*Buffer struct*/
typedef struct{
	short addr;
	byte access_type;
	byte init;
}buffer_slot;

/*Buffers*/
buffer_slot buffers [NUM_BUFFERS][BUFFER_SIZE];

/*Block Structure*/
typedef struct{
	short addr;
	struct block* next;
	struct block* prev;
}block;

/*FCB*/
typedef struct{
	char filename[NAME_LIMIT];
	/*0 0 0 0 0 0 (Write access) (Directory)*/
	byte bits;
	fcb* dirHead;
	block* blockhead;
	block* blocktail;
	fcb* next;
	fcb* prev;
	byte device_num;
}fcb;

/*Device*/
typedef struct{
	fcb* filehead;
	/*Memory size divided by smallest allowed block, divided by 8 bits per byte*/
	byte bitmap[MEM_SIZE / 32];
	byte numblock;
	char fs_name;
	/*0 0 0 0 0 0 (Mounted) (Formated)*/
	byte bits;
	char fsname;
}device;

/*Device list*/
device device_array [MAX_DEVICE];

typedef struct{
	fcb* file;
	byte open;
}open_type;
/*Open files array*/
open_type open_files[MAX_OPEN];

#endif
