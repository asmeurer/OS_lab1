/**
 * File Manager
 *
 * Group 2
 * Aaron Meurer
 * Oran Wallace
 * Sheng Lundquist
 */
#include "file_manager.h"

/* Init_fs: checks if the device is known, if so set the device name */
int init_fs (int device){
    int i;
    /*Unmount all devices*/
    for (i = 0; i < MAX_DEVICE; i++){
        device_array[i].bits = device_array[i].bits & (~DEVICE_MOUNTED_BITMASK);
    }
    /*Clear open file array*/
    for (i = 0; i < MAX_OPEN; i++){
        open_files[i].open = 0;
        open_files[i].file = null;
    }
    return ERROR_SUCCESS;
}

/* Mount: checks if the device has been inited and formated, if so mounts it. Otherwise, returns an error. */
int mount (char fs_name){
    int i;
    for(i = 0; i < MAX_DEVICE; i++){
        if(device_array[i].fs_name == fs_name && (!(device_array[i].bits & DEVICE_FORMAT_BITMASK))){
            device_array[i].bits | DEVICE_MOUNTED_BITMASK;
            return ERROR_SUCCESS;
        }
    }
    */
        return ERROR_NOT_INITIALIZED_OR_FORMATED;
}

int format(int device_num, char fs_name, int blocksize){
    /*Check for correct num of devices*/
    int i;

    if(blocksize != 4 || blocksize != 8 || blocksize != 16){
        return ERROR_INVALID_BLOCK_SIZE;
    }

    for(i = 0; i < MAX_DEVICE; i++){
        if(!(device_array[device_num].bits & DEVICE_MOUNTED_BITMASK)){
            device_array[device_num].bits | DEVICE_FORMAT_BITMASK;
            device_array[device_num].fs_name = fs_name;
        }else if(device_array[device_num].bits & DEVICE_MOUNTED_BITMASK){
            return ERROR_DEVICE_MOUNTED;
        }
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
    format_me->numblock = MEM_SIZE/blocksize;

    /*Erase*/
    for(i = 0; i < MEM_SIZE / 32; i++){
        format_me->bitmap[i] = 0;
    }
    format_me->bits = format_me->bits | DEVICE_FORMAT_BITMASK;


    if(device_num < 0 || device_num >= MAX_DEVICE){
        return ERROR_INVALID_DEVICE_NUM;
    }

    return format_me->numblock;
}

int unmount(char fs_name){
    return ERROR_SUCCESS;
}

/* Convert a file path into a file.  This is very similar to the search
 * inside create(). */
fcb *get_file(int dev, path *file_path)
{
    struct path *next = file_path;
    struct dir_queue_t *current_dir = device_array[dev].root;
    fcb *current_file = current_dir->tail;

    while (next->next != null) {
        if (current_file == null) {
            error_file.device_num = ERROR_DIR_IS_FILE;
            return &error_file;
        } else if (filename_eq(current_file->filename, next->string)) {
            if (next->next == null) {
                return current_file;
            } else if (!(current_file->bits & FCB_DIR_BITMASK)) {
                error_file.device_num = ERROR_DIR_IS_FILE;
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

int open(char fs_name, path *file_path, int write){
    int dev = get_device(fs_name);
    fcb *file = get_file(dev, file_path);
    int i;
    int found = 0;

    if (dev < 0) {
        /* There was an error in get_device() */
        return dev;
    }

    if (file->device_num < 0) {
        /* There was an error with get_file() */
        return file->device_num;
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

    return ERROR_SUCCESS;
}

int write(int filehandle, short block_number, int buf_ptr){
    block *temp;
    fcb *file;
    int error = 1;
    int i = 0;

    /*Check if file is open*/
    if (!(open_files[filehandle].bits & OPEN_TYPE_OPEN)){
        return ERROR_FILE_NOT_OPEN;
    }

    /*Check if block number is part of file*/

    file = open_files[filehandle].file;

    if(!(file->bits & FCB_DIR_BITMASK)){
        return ERROR_FILE_IS_DIR;
    }


    /*Check if buffer point is valid */



}

int read(int filehandle, short block_number, int buf_ptr){
    block* temp;
    int error = 1;
    int i = 0;
    /*Check if file is open*/
    if (!(open_files[filehandle].bits & OPEN_TYPE_OPEN)){
        return ERROR_FILE_NOT_OPEN;
    }

    temp = open_files[filehandle].file->blocktail;
    /*Check if block number is part of file*/
    while (temp != null){
        if (temp->addr == block_number){
            error = 0;
        }
        temp = temp->next;
    }
    if (error == 1){
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
    newfile->block_queue = new_blockqueue;
    newfile->device_num = dev;

    dir_enqueue(current_dir, newfile);

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
