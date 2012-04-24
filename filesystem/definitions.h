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

/*Constants*/
#define BUFFER_SIZE 5
#define MAX_DEVICE 5
#define MAX_OPEN 8
#define FCB_DIR_BITMASK 0x01
#define FCB_WRITE_ACC_BITMASK 0x02

/*Buffers*/
short rBuffers [MAX_DEVICE][BUFFER_SIZE];
short wBuffers [MAX_DEVICE][BUFFER_SIZE];
byte rBufSize [MAX_DEVICE];
byte wBufSize [MAX_DEVICE];

/*FCB*/
typedef struct{
	char filename[10];
	/*Goes 000000(Write access)(Directory)*/
	byte bits;
	struct fcb* dirHead;
	short* blockhead;
	short* blocktail;
	struct fcb* next;
	struct fcb* prev;
}fcb;

/*Open files array*/
fcb open_files[MAX_OPEN];

/*Open files queue bookkeeping*/
typedef struct{
	fcb *head;
	fcb *tail;
	fcb *top;
	int initialized;
}open_files_t;


#endif
