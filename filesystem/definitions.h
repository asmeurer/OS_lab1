/**
 * Filesystem Definitions header
 *
 * Group 2
 * Aaron Meurer
 * Oran Wallace
 * Sheng Lundquist
 */


#ifndef FILE_DEF_H
#define FILE_DEF_H

typedef unsigned char byte;

#define null 0

/* Error Codes
 *
 * Yes, there are a lot of them
 */
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
#define ERROR_FS_NAME_ARG -23
#define ERROR_BAD_OPTION -24
#define ERROR_TOO_MANY_OPEN_FILES -25
#define ERROR_NO_FREE_BLOCKS -26
#define ERROR_ADDR_OUT_OF_BOUNDS -27
#define ERROR_BLOCK_ALREADY_EMPTY -28
#define ERROR_BLOCK_ALREADY_FULL -29
#define ERROR_FILES_ARE_OPEN -30
#define ERROR_ALREADY_MOUNTED -31
#define ERROR_ALREADY_UNMOUNTED -32
#define ERROR_FS_NAME_NOT_EXISTS -33
#define ERROR_FILE_IS_READ_ONLY -34
#define ERROR_FILE_HANDLE_OUT_OF_RANGE -35

/* Constants
 *
 * Most of the "size" constants can be changed without any
 * issues.
 */
#define null 0
#define BUFFER_SIZE 5
#define MAX_DEVICE 5
#define MAX_OPEN 8              /* This is probably too small, but it makes
                                 * testing easier and it can be easily
                                 * changed. */
#define FCB_DIR_BITMASK 0x01
#define OPEN_TYPE_OPEN_BITMASK 0x01
#define OPEN_TYPE_WRITE_ACC_BITMASK 0x02
#define DEVICE_FORMAT_BITMASK 0x01
#define DEVICE_MOUNTED_BITMASK 0x02
#define NAME_LIMIT 11
#define NUM_BUFFERS 5
/*256 MB*/
#define MEM_SIZE (256<<20)

/* Memory size divided by smallest allowed block size (4KB), divided by 8 bits
     * per byte */

#define MAX_BLOCK_SIZE (MEM_SIZE/(4<<10))

enum rw {
    READ,
    WRITE
};

/* Buffer struct */
typedef struct{
    unsigned short addr;
    enum rw access_type;
    byte init;
} buffer_slot;

/* Buffers */
buffer_slot buffers [NUM_BUFFERS][BUFFER_SIZE];

/* Block struct */
struct block{
    unsigned short addr;
    struct block* next;
    struct block* prev;
    byte error;
};
typedef struct block block;

/* File Control Block struct */
struct fcb {
    char filename[NAME_LIMIT];
    /*0 0 0 0 0 0 0 (Directory)*/
    byte bits;
    struct dir_queue_t* dirHead;
    struct dir_queue_t* parent_dir;
    struct block_queue_t *block_queue;
    struct fcb* next;
    struct fcb* prev;
    byte device_num;
    int error;
};
struct fcb;
typedef struct fcb fcb;

/* Device struct */
typedef struct {
    fcb *root;
    byte bitmap[MAX_BLOCK_SIZE];
    byte numblock;
    char fs_name;
    /*0 0 0 0 0 0 (Mounted) (Formatted)*/
    byte bits;
} device;

/* Device list*/
device device_array [MAX_DEVICE];

/* Open File struct */
typedef struct {
    fcb* file;
    byte bits;
    /* 0 0 0 0 0 0 (write access) (open) */
} open_type;

/*Open files array*/
open_type open_files[MAX_OPEN];

/* A simple linked list of strings to represent a file path.  This is what the
 * test runner passes to the filesystem manager functions. */
struct path
{
    char string[NAME_LIMIT];
    struct path *next;
};
struct path;
typedef struct path path;

#endif
