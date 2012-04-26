/**
 * File Manager
 *
 * Group 2
 * Aaron Meurer
 * Oran Wallace
 * Sheng Lundquist
 */

#include "file_manager.h"

/**
 * Checks if the device is known. If so, it sets the device name.
 *
 * @param device The integer index for the device to initialize
 *
 * @return Returns ERROR_SUCCESS on success, else an error code.
 */
int init_fs (int device){
    int i;
    /*Unmount all devices*/
    for (i = 0; i < MAX_DEVICE; i++){
        device_array[i].bits = device_array[i].bits & (~DEVICE_MOUNTED_BITMASK);
    }
    /*Clear open file array*/
    for (i = 0; i < MAX_OPEN; i++){
        open_files[i].bits = open_files[i].bits | (~OPEN_TYPE_OPEN_BITMASK);
        open_files[i].file = null;
    }
    return ERROR_SUCCESS;
}

/**
 * Checks if the device has been inited and formated, and if so mounts
 * it. Otherwise, returns an error.
 *
 * @param fs_name The character name of the filesystem to mount
 *
 * @return Returns ERROR_SUCCESS on success, else an error code.
 */
int mount (char fs_name){
    int i;
    for(i = 0; i < MAX_DEVICE; i++){
        if(device_array[i].fs_name == fs_name && (!(device_array[i].bits & DEVICE_FORMAT_BITMASK))){
            device_array[i].bits |= DEVICE_MOUNTED_BITMASK;
            return ERROR_SUCCESS;
        }
    }
    return ERROR_NOT_INITIALIZED_OR_FORMATED;
}

/**
 * Formats the given device.
 *
 * This gives the device a character name, erases all data on the device, and
 * breaks it up into blocks.  This only works if the device is unmounted.
 */
int format(int device_num, char fs_name, int blocksize){
    /*Check for correct num of devices*/
    int i;

    if(blocksize != 4 || blocksize != 8 || blocksize != 16){
        return ERROR_INVALID_BLOCK_SIZE;
    }

    if(device_num < 0 || device_num >= MAX_DEVICE){
        return ERROR_INVALID_DEVICE_NUM;
    }
    device *format_me = &device_array[device_num];
    /*Must be unmounted*/
    if(format_me->bits & DEVICE_MOUNTED_BITMASK){
        return ERROR_DEVICE_MOUNTED;
    }

    format_me->fs_name = fs_name;
    format_me->numblock = MEM_SIZE/(blocksize<<10);

    /*Erase*/
    for(i = 0; i < MEM_SIZE / 32; i++){
        format_me->bitmap[i] = 0;
    }
    format_me->bits = format_me->bits | DEVICE_FORMAT_BITMASK;

    return format_me->numblock;
}

int unmount(char fs_name){
    int i;
    for(i = 0; i < MAX_DEVICE; i++){

    }
    return ERROR_SUCCESS;
}

/* Convert a file path into a file.  This is very similar to the search inside
 * create(). */
fcb *get_file(int dev, path *file_path)
{
    struct path *next = file_path;
    struct dir_queue_t *current_dir = device_array[dev].root;
    fcb *current_file = current_dir->tail;

    while (next->next != null) {
        if (current_file == null) {
            error_file.error = ERROR_DIR_IS_FILE;
            return &error_file;
        } else if (filename_eq(current_file->filename, next->string)) {
            if (next->next == null) {
                return current_file;
            } else if (!(current_file->bits & FCB_DIR_BITMASK)) {
                error_file.error = ERROR_DIR_IS_FILE;
                return &error_file;
            } else {
                next = next->next;
                current_dir = current_file->dirHead;
                current_file = current_dir->tail;
            }
        } else {
            current_file = current_file->next;
        }
    }

    return null;
}

/**
 * This function closes a file by using the file handle to referance it, then sets the fbc pointer file to null and the bits to 0
 */
int close(int filehandle){
    int i;
    for(i = 0; i < MAX_OPEN; i++){
        if(i == filehandle){
            open_files[i].file = null;
            open_files[i].bits = 0;
        }
    }
    return ERROR_SUCCESS;
}

int open(char fs_name, path *file_path, int write){
    int dev = get_device(fs_name);
    fcb *file = get_file(dev, file_path);
    int i;
    int found = 0;

    if (dev < 0) {
        /* There was an error in get_device() */
        return dev;
    }

    if (file->error < 0) {
        /* There was an error with get_file() */
        return file->error;
    }

    /* Find and empty open file slot */
    for (i = 0; i < MAX_OPEN; i++) {
        if (!(open_files[i].bits & OPEN_TYPE_OPEN_BITMASK)) {
            found = 1;
            break;
        }
    }

    if (!found) {
        return ERROR_TOO_MANY_OPEN_FILES;
    }

    open_files[i].file = file;
    open_files[i].bits = 0;
    open_files[i].bits |= OPEN_TYPE_OPEN_BITMASK;
    if (write) {
        open_files[i].bits |= OPEN_TYPE_WRITE_ACC_BITMASK;
    }

    return i;
}

int write(int filehandle, short block_number, int buf_ptr){
    block *temp;
    fcb *file;
    int error;
    int i = 0;

    /*Check if file is open*/
    if (!(open_files[filehandle].bits & OPEN_TYPE_OPEN_BITMASK)){
        return ERROR_FILE_NOT_OPEN;
    }

    /*Check if block number is part of file*/
    file = open_files[filehandle].file;
    if(!(file->bits & FCB_DIR_BITMASK)){
        return ERROR_FILE_IS_DIR;
    }

    /* Check if the block number is associated with another file, if not set the block */
    error = set_block_full(file->device_num, block_number);
    if(error != ERROR_SUCCESS){
        return error;
    }

    /* Malloc the block, enqueue it to the block_queue and check for any errors */
    temp = malloc_block();
    temp->addr = block_number;
    error = block_enqueue(file->block_queue, temp);
    if(error != ERROR_SUCCESS){
        return error;
    }

    /*Find next buffer slot*/
    for(i = 0; i < BUFFER_SIZE; i++){
        if(buffers[buf_ptr]->init == 0){
            buffers[buf_ptr]->init = 1;
            buffers[buf_ptr]->addr = block_number;
            buffers[buf_ptr]->access_type = WRITE;
            break;
        }
        if (i == BUFFER_SIZE){
            return ERROR_BUFFER_FULL;
        }
    }

    return ERROR_SUCCESS;

}

int read(int filehandle, short block_number, int buf_ptr){
    fcb *file;

    int i = 0;
    /*Check if file is open*/
    if(filehandle < 0 || filehandle >= MAX_OPEN){
		return ERROR_FILE_HANDLE_OUT_OF_RANGE;
	}
    if (!(open_files[filehandle].bits & OPEN_TYPE_OPEN_BITMASK)){
        return ERROR_FILE_NOT_OPEN;
    }
    file = open_files[filehandle].file;
    /* Check if file is a direcroty */
    if(!(file->bits & FCB_DIR_BITMASK)){
        return ERROR_FILE_IS_DIR;
    }
    /*Check if block number is part of file*/
    if((seach_blocks(file->block_queue, block_number)) != 1){
        return ERROR_BLOCK_NOT_IN_FILE;
    }
    /*Check buffer pointer*/
    if(buf_ptr < 0 || buf_ptr >= NUM_BUFFERS){
        return ERROR_BUFFER_NOT_EXIST;
    }
    /*Find next buffer slot*/
    for(i = 0; i < BUFFER_SIZE; i++){
        if(buffers[buf_ptr]->init == 0){
            buffers[buf_ptr]->init = 1;
            buffers[buf_ptr]->addr = block_number;
            buffers[buf_ptr]->access_type = READ;
            break;
        }
        if (i == BUFFER_SIZE){
            return ERROR_BUFFER_FULL;
        }
    }
    return ERROR_SUCCESS;
}

int create(char fs_name, struct path *file_path, int dir)
{
    int dev = get_device(fs_name);

    struct path *next = file_path;
    struct dir_queue_t *current_dir = device_array[dev].root;
    fcb *current_file = current_dir->tail;

    fcb *newfile;
    struct dir_queue_t *new_dirqueue;
    struct block_queue_t *new_blockqueue;

    while (next->next != null) {
        if (current_file == null) {
            /* We reached the end of the directory and didn't find the
             * specified directory in the path.  Since create_file does not
             * recursively create directories, this is an error. */
            return ERROR_DIR_NOT_FOUND;

        } else if (filename_eq(current_file->filename, next->string)) {
            if (next->next == null) {
                /* End of the given path; this is the file to create.  We
                 * found the file, so this means it's an error. */
                return ERROR_FILE_ALREADY_EXISTS;
            } else if (!(current_file->bits & FCB_DIR_BITMASK)) {
                /* One of the directories in the path is actually a file.
                 * This is also an error. */
                return ERROR_DIR_IS_FILE;
            } else {
                /* We found one of the directories in the given path.
                 * Continue recursing. */
                next = next->next;
                current_dir = current_file->dirHead;
                current_file = current_dir->tail;
            }

        } else {
            current_file = current_file->next;
        }
    }
    /* If we reach the end of the while loop, it means that the given path
     * already exists, and the given file does not.  So create the file.*/
    newfile = malloc_file();
    new_blockqueue = malloc_block_queue();
    block_init_queue(new_blockqueue);

    if (dir) {
        new_dirqueue = malloc_dir_queue();
        dir_init_queue(new_dirqueue);
    } else {
        new_dirqueue = null;
    }

    block_init_queue(new_blockqueue);

    filename_copy(newfile->filename, next->string);
    newfile->bits = dir;
    newfile->dirHead = new_dirqueue;
    newfile->containing_dir = current_dir;
    newfile->block_queue = new_blockqueue;
    newfile->device_num = dev;
    newfile->error = 0;

    dir_enqueue(current_dir, newfile);

    return ERROR_SUCCESS;
}

int delete(char fs_name, struct path *file_path)
{
    int error;
    int dev = get_device(fs_name);
    fcb *file = get_file(dev, file_path);
    block *file_block = null;

    /* If the file is a directory, delete it if and only if it is empty. */
    if (file->bits | FCB_DIR_BITMASK) {
        if (file->dirHead == null) {
            file = dir_delete(file->containing_dir, file);
            if (file->error < 0) {
                return file->error;
            }
            Free(file);
        } else {
            /* We have to support recursively deleting a directory (unlike we
             * originally planned), because format() needs to be able to do this. */
            while (
        }
    } else {
        file = dir_delete(file->containing_dir, file);
        if (file->error < 0) {
            return file->error;
        }
        /* Clear all the blocks in the file.  This means setting the addresses
         * as free in the bitmap and actually clearing the "memory" of those
         * blocks. */
        file_block = block_dequeue(file->block_queue);
        while (file_block != null) {
            error = set_block_empty(dev, file_block->addr) < 0;
            if (error < 0) {
                return error;
            }
            Free(file_block);
            file_block = block_dequeue(file->block_queue);
        }
    }
    return ERROR_SUCCESS;
}

/* Convert a filesystem character name to a device number */
int get_device(char fs_name)
{
    int i;
    for (i=0; i < MAX_DEVICE; i++) {
        if (device_array[i].fs_name == fs_name) {
            return i;
        }
    }
    return ERROR_BAD_FS_NAME;
}

/* Custom version of strcmp to compare filenames. Returns 0 if the names are
 * unequal and 1 if they are equal. */
int filename_eq(char *string1, char *string2)
{
    int i;
    for (i = 0; i < NAME_LIMIT; i++) {
        if (string1[i] != string2[i]) {
            return 0;
        }
    }
    return 1;
}

/* Custom version of strcpy for filenames. Copies the string from source into
 * dest.  Doesn't do any error checking. */
void filename_copy(char *source, char *dest)
{
    int i;
    for (i = 0; i < NAME_LIMIT; i++) {
        dest[i] = source[i];
    }
}


/**
 * Find a free block slot in the device.  This will correspond to a 0 bit in
 * the blocks_free byte array.
 * @param dev Device number to search.
 * @return Returns the index of the empty backing frame.
 */
short find_empty_block(int dev) {
    int prefix;
    const int blocks_free_size = MAX_BLOCK_SIZE/8;
    byte not_byte;
    int found = 0;
    int suffix = 0;
    int shift_mask;

    for (prefix = 0; prefix < blocks_free_size; prefix++) {
        if (device_array[dev].bitmap[prefix] != 0xff) {
            found = 1;
            not_byte = ~device_array[dev].bitmap[prefix];
            break;
        }
    }

    /* We got to the end of the loop and didn't find any free slots. */
    if (!found) {
        return ERROR_NO_FREE_BLOCKS;
    }

    /* We have the byte that has a 0 in it, now find where the first 0 is.  To
     * check this, we create a bitmask 0b1000000 and shift it right until we
     * find the first 1 bit. */
    shift_mask = 0x80;
    for (suffix = 0; suffix < 8; suffix++){
        if (not_byte & shift_mask) {
            break;
        }
        shift_mask >>= 1;
    }

    /* Finally, convert the prefix and suffix into an address. */
    return (prefix << 3) + suffix;
}


/**
 * Set the block address addr to full.  Returns errors if the block is
 * already in the state it is being set to, or if the address is out of
 * bounds.
 * @param dev The device to set.
 * @param addr The address to be set as empty.
 * @return Returns an error code.
 */
int set_block_empty(int dev, short addr) {
    int prefix;
    int suffix;
    byte suffix_bitmask;

    if (addr > device_array[dev].numblock) {
        return ERROR_ADDR_OUT_OF_BOUNDS;
    }

    prefix = addr >> 3;
    suffix = addr & 0x7;
    /* Create the bitmask for the suffix */
    suffix_bitmask = 1 << (7 - suffix);

    if (~device_array[dev].bitmap[prefix] & suffix_bitmask) {
        /* The memory was already set to empty */
        return ERROR_BLOCK_ALREADY_EMPTY;
    } else {
        device_array[dev].bitmap[prefix] = ~(~device_array[dev].bitmap[prefix] | suffix_bitmask);
        return ERROR_SUCCESS;
    }
}

/**
 * Set the address addr to empty. Returns errors if the memory is
 * already in the state it is being set to, or if the address is out of
 * bounds.
 * @param dev The device to set.
 * @param addr The address to be set as full.
 * @return Returns an error code.
 */
int set_block_full(int dev, short addr) {
    int prefix;
    int suffix;
    byte suffix_bitmask;

    if (addr > device_array[dev].numblock) {
        return ERROR_ADDR_OUT_OF_BOUNDS;
    }

    prefix = addr >> 3;
    suffix = addr & 0x7;
    /* Create the bitmask for the suffix */
    suffix_bitmask = 1 << (7 - suffix);

    if (device_array[dev].bitmap[prefix] & suffix_bitmask) {
        /* The block was already set to full */
        return ERROR_BLOCK_ALREADY_FULL;
    } else {
        device_array[dev].bitmap[prefix] = device_array[dev].bitmap[prefix] | suffix_bitmask;
        return ERROR_SUCCESS;
    }
}
