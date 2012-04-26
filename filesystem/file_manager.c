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
 * Initialize the filesystem manager data structures.
 *
 * @param device The integer index for a device.
 *
 * @return Returns ERROR_SUCCESS on success, else an error code.
 */
int init_fs(int device){
    int i;
    /* Unmount all devices */
    for (i = 0; i < MAX_DEVICE; i++){
        device_array[i].bits = device_array[i].bits & (~DEVICE_MOUNTED_BITMASK);
    }

    /* Clear open file array. */
    for (i = 0; i < MAX_OPEN; i++){
        open_files[i].bits = open_files[i].bits | (~OPEN_TYPE_OPEN_BITMASK);
        open_files[i].file = null;
    }

    /* Clear all the buffers. */
    for (i = 0; i < NUM_BUFFERS; i++) {
        buf_flush(i);
    }

    device_array[device].bits = device_array[device].bits | DEVICE_INIT_BITMASK;
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
int mount(char fs_name){
    int i;
    int dev;
    dev = get_device(fs_name);

    if (dev < 0) {
        /* Bad fs_name */
        return dev;
    }

    /* Mount cannot work if the file system has not been formatted, or if it
     * is already mounted. */
    if (!(device_array[dev].bits & DEVICE_FORMAT_BITMASK)) {
        /* A file system cannot have a name if it isn't formatted, so this
         * should actually never happen. */
        return ERROR_NOT_INITIALIZED_OR_FORMATED;
    }

    if (device_array[dev].bits & DEVICE_MOUNTED_BITMASK) {
        return ERROR_ALREADY_MOUNTED;
    }

    device_array[dev].bits |= DEVICE_MOUNTED_BITMASK;

    for (i = 1; i < NUM_BUFFERS; i++) {
    }

    return ERROR_SUCCESS;
}

/**
 * Formats the given device.
 *
 * This gives the device a character name, erases all data on the device (all
 * files, directories, and blocks), and breaks it up into blocks.  It also
 * creates the root directory. This only works if the device is unmounted.
 *
 * @param device_num The device number to be formatted.  Must be between 0 and
 * MAX_DEVICE - 1.
 *
 * @param fs_name The character name for the filesystem.  Must be a single
 * uppercase letter (A-Z).
 *
 * @param blocksize The size of the blocks, in KB.  Must be one of 4, 8, or
 * 16.
 *
 * @return Returns the number of blocks on success, else the error code.
 */
int _format(int device_num, char fs_name, int blocksize){
    /*Check for correct num of devices*/
    int i;
    int error = 0;

    if (blocksize != 4 && blocksize != 8 && blocksize != 16){
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
    if(!(format_me->bits & DEVICE_INIT_BITMASK)){
		return ERROR_DEVICE_NOT_KNOWN;
	}

    if (fs_name < 'A' || fs_name > 'Z') {
        return ERROR_BAD_FS_NAME;
    }

    format_me->fs_name = fs_name;
    format_me->numblock = MEM_SIZE/(blocksize<<10);
    if (format_me->root != null && !(format_me->bits & DEVICE_INIT_BITMASK)) {
        /* The device has been used since being formatted last. */
        error = delete_internal(device_num, format_me->root);
    }

    if (error < 0) {
        //return error;
    }

    /*Erase*/
    for(i = 0; i < MEM_SIZE / (blocksize << 10); i++){
        format_me->bitmap[i] = 0;
    }
    format_me->bits = format_me->bits | DEVICE_FORMAT_BITMASK;

    format_me->root = malloc_file();
    filename_copy(format_me->root->filename,"root");
    format_me->root->bits = 0;
    format_me->root->dirHead = malloc_dir_queue();

    dir_init_queue(format_me->root->dirHead);

    /* This is the reason we don't use create() to make the root directory.
     * It has a special case for parent_dir, which is just null. */
    format_me->root->parent_dir = null;
    format_me->root->block_queue = null;
    format_me->root->next = null;
    format_me->root->prev = null;
    format_me->root->device_num = device_num;
    format_me->root->error = 0;

    return format_me->numblock;
}

/**
 * Unmounts the drive.
 *
 * A drive cannot be unmounted if it has open files, or if it is already
 * unmounted.
 *
 * @param fs_name The name of the filesystem to unmount.
 *
 * @return Returns ERROR_SUCCESS on success, else an error code.
 */
int unmount(char fs_name){
    int dev = get_device(fs_name);
    int i;
    int j;

    if (dev < 0) {
        /* Bad fs_name */
        return dev;
    }

    /* A device cannot be unmounted if it is not formatted, or if it is
     * already unmounted. */
    if (!(device_array[dev].bits & DEVICE_FORMAT_BITMASK)) {
        /* This should never happen. */
        return ERROR_NOT_INITIALIZED_OR_FORMATED;
    }

    if (!(device_array[dev].bits & DEVICE_MOUNTED_BITMASK)) {
        return ERROR_ALREADY_UNMOUNTED;
    }

    /* A device cannot be unmounted if there are open files. */
    for(i = 0; i < MAX_OPEN; i++){
        if ((open_files[i].bits & OPEN_TYPE_OPEN_BITMASK) && (open_files[i].file->device_num == dev)) {
            return ERROR_FILES_ARE_OPEN;
        }
    }

    /* Before a device can be unmounted, the buffers must be cleared.  This
     * calls the hardware simulation of flushing the buffers that have buffer
     * slots corresponding to this device.  In a real system, this would
     * amount to blocking the unmount until all such buffers have been
     * cleared. */
    for (i = 0; i < NUM_BUFFERS; i++) {
        for (j = 0; j < BUFFER_SIZE; j++) {
            if (buffers[i][j].device_num == dev) {
                /* This buffer has a slot corresponding to this device, so
                 * flush it.  There's no need to check for more; just one is
                 * enough to warrant this. */
                buf_flush(i);
                break;
            }
        }
    }

    device_array[dev].bits |= ~DEVICE_MOUNTED_BITMASK;
    return ERROR_SUCCESS;
}

/**
 * Convert a file path into a file.
 *
 * This is used internally to find the file given a file path data structure,
 * which is basically a linked list of strings.
 *
 * @param dev The device number to look for the file in
 *
 * @file_path The file path corresponding to the file to be returned.
 *
 * @return Returns a pointer to the file corresponding to file_path.  Any
 * error codes will be in the error field of the fcb.
 */
fcb *get_file(int dev, path *file_path)
{
    if (!(device_array[dev].bits & DEVICE_FORMAT_BITMASK)) {
        error_file.error = ERROR_NOT_INITIALIZED_OR_FORMATED;
        return &error_file;
    }

    /* This is very similar to the search inside create(), but we care about
     * different errors in this case. */
    struct path *next = file_path;
    struct dir_queue_t *current_dir = device_array[dev].root->dirHead;
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

    return current_file;
}

/**
 * Closes the given file, by file handle.
 *
 * The file handle is the index in the open file array (though this is
 * technically an implementation detail).
 *
 * @param filehandle The file handle for the file to be closed.  This is the
 * same file handle that will be returned to the user on a call to open().
 *
 * @return Returns ERROR_SUCCESS on success, else an error code.
 */
int close(int filehandle){
    /* Check the bounds of the filehandle */
    if (filehandle < 0 || filehandle >= MAX_OPEN) {
        return ERROR_FILE_HANDLE_OUT_OF_RANGE;
    }

    /* Check if file is open */
    if (!(open_files[filehandle].bits & OPEN_TYPE_OPEN_BITMASK)){
        return ERROR_FILE_NOT_OPEN;
    }

    /* TODO: Check if the file is using any buffers. */

    open_files[filehandle].file = null;
    open_files[filehandle].bits = 0;

    return ERROR_SUCCESS;
}

/**
 * Opens the given file on the given filesystem, with read-write or read-only
 * permissions.
 *
 * @param fs_name The filesystem name where the file is.
 *
 * @param file_path The file path data structure corresponding to the file to
 * be opened.
 *
 * @param write A boolean, 1 means open the file with read-write permissions,
 * 0 means open the file with read-only permissions.
 *
 * @return Returns the file handle on success, or an error code on failure.
 * The file handle is an integer that can be passed to close() to close the
 * file.  In the implementation, it is just the index in the open file array.
 */
int open(char fs_name, path *file_path, int write){

    int dev = get_device(fs_name);
    if (dev < 0) {
        /* There was an error in get_device() */
        return dev;
    }

    fcb *file = get_file(dev, file_path);
    int i;
    int found = 0;

    if (file->error < 0) {
        /* There was an error with get_file() */
        return file->error;
    }

    if (file->bits & FCB_DIR_BITMASK) {
        return ERROR_FILE_IS_DIR;
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

/**
 * Writes the block block_number to the file corresponding to filehandle using
 * the buffer corresponding to buf_ptr.
 *
 * @param filehandle The file handle of the file to write. This is the number
 * returned by open().
 *
 * @param block_number The address of the block to be written.
 *
 * @param buf_ptr The index of the buffer to use for writing.
 *
 * @return Returns ERROR_SUCCESS on success and an error code otherwise.
 */
int write(int filehandle, unsigned short block_number, int buf_ptr){
    block *temp;
    fcb *file;
    int error;
    int i = 0;

    /* Check the bounds of the filehandle */
    if (filehandle < 0 || filehandle >= MAX_OPEN) {
        return ERROR_FILE_HANDLE_OUT_OF_RANGE;
    }

    /* Check the bounds on the buf_ptr */
    if (buf_ptr < 0 || buf_ptr >= NUM_BUFFERS) {
        return ERROR_BUFFER_NOT_EXIST;
    }

    /* Check if file is open */
    if (!(open_files[filehandle].bits & OPEN_TYPE_OPEN_BITMASK)){
        return ERROR_FILE_NOT_OPEN;
    }
    file = open_files[filehandle].file;

    /* Check if the file has write permissions. */
    if (!(file->bits & OPEN_TYPE_WRITE_ACC_BITMASK)) {
        return ERROR_FILE_IS_READ_ONLY;
    }

    /* Check if file is a directory */
    if(!(file->bits & FCB_DIR_BITMASK)){
        return ERROR_FILE_IS_DIR;
    }

    /* Check if the block number is associated with another file. If not set the block. */
    error = set_block_full(file->device_num, block_number);
    if(error != ERROR_SUCCESS){
        return error;
    }

    /* Malloc the block, enqueue it to the block_queue, and check for any errors */
    temp = malloc_block();
    temp->addr = block_number;
    error = block_enqueue(file->block_queue, temp);
    if(error != ERROR_SUCCESS){
        return error;
    }

    /*Find next buffer slot*/
    for(i = 0; i < BUFFER_SIZE; i++){
        if(buffers[buf_ptr][i].init == 0){
            buffers[buf_ptr][i].init = 1;
            buffers[buf_ptr][i].addr = block_number;
            buffers[buf_ptr][i].access_type = WRITE;
            buffers[buf_ptr][i].device_num = file->device_num;
            break;
        }
        if (i == BUFFER_SIZE){
            return ERROR_BUFFER_FULL;
        }
    }

    return ERROR_SUCCESS;

}

/**
 * Reads the block block_number from the file corresponding to filehandle
 * using the buffer corresponding to buf_ptr.
 *
 * @param filehandle The file handle of the file to read from. This is the
 * number returned by open().
 *
 * @param block_number The address of the block to be read.
 *
 * @param buf_ptr The index of the buffer to use for reading.
 *
 * @return Returns ERROR_SUCCESS on success and an error code otherwise.
 */
int read(int filehandle, unsigned short block_number, int buf_ptr){
    fcb *file;

    int i = 0;

    /* Check the bounds of the filehandle */
    if (filehandle < 0 || filehandle >= MAX_OPEN) {
        return ERROR_FILE_HANDLE_OUT_OF_RANGE;
    }

    /* Check the bounds on the buf_ptr */
    if (buf_ptr < 0 || buf_ptr >= NUM_BUFFERS) {
        return ERROR_BUFFER_NOT_EXIST;
    }

    /*Check if file is open*/
    if (!(open_files[filehandle].bits & OPEN_TYPE_OPEN_BITMASK)){
        return ERROR_FILE_NOT_OPEN;
    }
    file = open_files[filehandle].file;

    /* Check if file is a directory */
    if(!(file->bits & FCB_DIR_BITMASK)){
        return ERROR_FILE_IS_DIR;
    }

    /* Check if block number is part of file */
    if((seach_blocks(file->block_queue, block_number)) != 1){
        return ERROR_BLOCK_NOT_IN_FILE;
    }

    /* Find next buffer slot */
    for(i = 0; i < BUFFER_SIZE; i++){
        if(buffers[buf_ptr][i].init == 0){
            buffers[buf_ptr][i].init = 1;
            buffers[buf_ptr][i].addr = block_number;
            buffers[buf_ptr][i].access_type = READ;
            buffers[buf_ptr][i].device_num = file->device_num;
            break;
        }
        if (i == BUFFER_SIZE){
            return ERROR_BUFFER_FULL;
        }
    }
    return ERROR_SUCCESS;
}

/**
 * Create the given file or directory.
 *
 * Directories are not create recursively---all sub-directories in the given
 * path must already exist.
 *
 * @param fs_name The name of the filesystem that the file should be created
 * in.
 *
 * @param file_path The file path data structure corresponding to the file or
 * directory to be created.
 *
 * @param dir A boolean.  If it is 1, it means the new file should be a
 * directory; if it is 0 it means that the new file should be a file.
 *
 * @return Returns ERROR_SUCCESS on success and an error code on failure.
 */
int create(char fs_name, struct path *file_path, int dir)
{
    int dev = get_device(fs_name);

    if (dev < 0) {
        /* Bad fs_name */
        return dev;
    }

    struct path *next = file_path;
    struct dir_queue_t *current_dir = device_array[dev].root->dirHead;
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
    newfile->parent_dir = current_dir;
    newfile->block_queue = new_blockqueue;
    newfile->device_num = dev;
    newfile->error = 0;

    dir_enqueue(current_dir, newfile);

    return ERROR_SUCCESS;
}

/**
 * Deletes the given file or directory.
 *
 * Directories are recursively deleted. All blocks for deleted files are
 * files.
 *
 * @param fs_name The filesystem where the file to be deleted is.
 *
 * @param file_path The path data structure for the file to be deleted.
 *
 * @return Returns ERROR_SUCCESS on success and an error code on failure.
 */
int delete(char fs_name, struct path *file_path)
{
    int dev = get_device(fs_name);
    if (dev < 0) {
        return dev;
    }

    fcb *file = get_file(dev, file_path);
    if (file->error < 0) {
        return file->error;
    }

    return delete_internal(dev, file);
}

/**
 * Internal function that does the heavy work for delete().
 */
int delete_internal(int dev, fcb *file)
{
    int error;
    block *file_block = null;
    fcb *next_file;

    /* If the file is a directory, delete it if and only if it is empty. */
    if (file->bits | FCB_DIR_BITMASK) {
        if (file->dirHead == null) {
            if (file->parent_dir == null) {
                /* The root directory */
                Free(file);
            } else {
                /* The directory is empty.  Just delete it. */
                file = dir_delete(file->parent_dir, file);

                if (file->error < 0) {
                    return file->error;
                }
                Free(file);
            }
        } else {
            /* We have to support recursively deleting a directory (unlike we
             * originally planned), because format() needs to be able to do
             * this. */
            next_file = dir_dequeue(file->dirHead);
            while (next_file != null) {
                delete_internal(dev, next_file);
                next_file = dir_dequeue(file->dirHead);
            }
        }
    } else {
        /* If the file is a file (not a directory), then delete all the data
         * (blocks), in addition to the fcb. */
        file = dir_delete(file->parent_dir, file);
        if (file->error < 0) {
            return file->error;
        }
        /* Clear all the blocks in the file.  This means setting the addresses
         * as free in the bitmap and actually clearing the "data" of those
         * blocks by calling Free in the hardware simulator (which actually
         * only frees the data structures associated with the blocks). */
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

/**
 * Convert a filesystem character name to a device number.
 *
 * Used internally.  The public API uses the filesystem name, and the private
 * API uses the device number.  This function converts one to the other.
 *
 * The filesystem character must be an uppercase letter (A-Z).
 *
 * @param fs_name The filesystem name to be converted.
 *
 * @return Returns the device number of success and an error code on failure.
 */
int get_device(char fs_name)
{
    int i;

    if (fs_name < 'A' || fs_name > 'Z') {
        return ERROR_BAD_FS_NAME;
    }

    for (i = 0; i < MAX_DEVICE; i++) {
        if (device_array[i].fs_name == fs_name) {
            return i;
        }
    }

    return ERROR_FS_NAME_NOT_EXISTS;
}

/**
 * Custom version of strcmp to compare filenames.
 *
 * This is here because we cannot use the system call version.  It has the
 * bounds check for the size of a filename built-in.
 *
 * @param string1 The first string to be compared.
 *
 * @param string2 The second string to be compared.
 *
 * @return Returns 0 if the string1 and string2 are unequal and 1 if they are
 * equal.
 */
int filename_eq(char *string1, char *string2)
{
    int i;
    for (i = 0; i < NAME_LIMIT; i++) {
        if (string1[i] != string2[i]) {
            return 0;
        }
        if (string1[i] == '\0' || string2[i] == '\0') {
            return 1;
        }
    }
    /* If we reach this, it's probably bad, because it means the strings
     * don't end with null terminators.  But we'll let someone else worry
     * about that. */
    return 1;
}

/**
 * Custom version of strcpy for filenames.  Copies the string from source into
 * dest.  Doesn't do any error checking.  It has the filename length bounds
 * check built-in.
 *
 * @param source The string to be copied into.
 *
 * @param dest The string to be copied.
 */
void filename_copy(char *source, char *dest)
{
    int i;
    for (i = 0; i < NAME_LIMIT; i++) {
        source[i] = dest[i];
    }
}


/**
 * Find a free block slot in the device.  This will correspond to a 0 bit in
 * the bitmap byte array.  This is the same function from the memory manager
 * used to find empty slots in the backing store.
 *
 * @param dev Device number to search.
 *
 * @return Returns the address of the empty block.
 */
unsigned short find_empty_block(int dev) {
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
 * Set the block address addr to full.  Returns errors if the block is already
 * in the state it is being set to, or if the address is out of bounds.  This
 * is the same function from the memory manager to set a frame as empty in the
 * backing store.
 *
 * @param dev The device to set.
 *
 * @param addr The address to be set as empty.
 *
 * @return Returns ERROR_SUCCESS on success or an error code on failure.
 */
int set_block_empty(int dev, unsigned short addr) {
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
 * Set the block address addr to empty. Returns errors if the memory is
 * already in the state it is being set to, or if the address is out of
 * bounds.  This is the same function from the memory manager to set a frame
 * as full in the backing store.
 *
 * @param dev The device to set.
 *
 * @param addr The address to be set as full.
 *
 * @return Returns ERROR_SUCCESS on success and an error code on failure.
 */
int set_block_full(int dev, unsigned short addr) {
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
