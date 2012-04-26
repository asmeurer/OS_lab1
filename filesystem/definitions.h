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

#include "file_queuemanager.h"

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
#define ERROR_BAD_DIR_QUEUE -11
#define ERROR_DIR_QUEUE_EMPTY -12
#define ERROR_ARG_NOT_INT -13
#define ERROR_BAD_BLOCK_PTR -14 /* Not to be confused with bad blocks */
#define ERROR_BAD_BLOCK_QUEUE -15
#define ERROR_BLOCK_QUEUE_EMPTY -16
#define ERROR_BAD_FS_NAME -17
#define ERROR_FILE_ALREADY_EXISTS -18
#define ERROR_DIR_IS_FILE -19
#define ERROR_DIR_NOT_FOUND -20
#define ERROR_INVALID_BLOCK_SIZE -21
#define ERROR_FILE_IS_DIR -22

/*Constants*/
#define null 0
#define BUFFER_SIZE 5
#define MAX_DEVICE 5
#define MAX_OPEN 8
#define FCB_DIR_BITMASK 0x01
#define OPEN_TYPE_OPEN 0x01
#define OPEN_TYPE_WRITE_ACC_BITMASK 0x02
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

struct block{
	unsigned short addr;
	struct block* next;
	struct block* prev;
    byte error;
};
/*Block Structure*/
typedef struct block block;

/*FCB*/

struct fcb {
	char filename[NAME_LIMIT];
	/*0 0 0 0 0 0 0 (Directory)*/
	byte bits;
	struct dir_queue_t* dirHead;
    struct block_queue_t *block_queue;
	fcb* next;
	fcb* prev;
	byte device_num;
};
struct fcb;
typedef struct fcb fcb;

/*Device*/
typedef struct{
	struct dir_queue_t *root;
	/*Memory size divided by smallest allowed block, divided by 8 bits per byte*/
	byte bitmap[MEM_SIZE / 32];
	byte numblock;
	char fs_name;
	/*0 0 0 0 0 0 (Mounted) (Formated)*/
	byte bits;
}device;

/*Device list*/
device device_array [MAX_DEVICE];

typedef struct{
	fcb* file;
	byte bits;
    /* 0 0 0 0 0 0 (write access) (open) */
}open_type;
/*Open files array*/
open_type open_files[MAX_OPEN];

/* A simple linked list of strings to represent a file path */
struct path
{
    char string[NAME_LIMIT];
    struct path *next;
    struct path *prev;
};

#endif
