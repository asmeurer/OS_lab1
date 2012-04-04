/**
 * Memory OS Manager
 */
#include "memory_manager.h"

/* Find a free memory slot in the backing store.  This will correspond to a 0
 * bit in the backing_store_free byte array. */
short find_empty_back_addr() {
    int prefix;
    const int backing_store_free_size = BACK_STORE_SIZE/8;
    byte not_byte;
    int found = 0;
    int suffix = 0;

    for (prefix = 0; prefix < backing_store_free_size; prefix++) {
        if (~backing_store_free[prefix]) {
            /* A 0 bit corresponds to a free slot, so ! (logical not) will be
             * true if there is at least one 0 and false if it is all 1's. */
            found = 1;
            not_byte = ~backing_store_free[prefix];
            break;
        }
    }

    /* We got to the end of the loop and didn't find any free slots. */
    if (!found) {
        return NO_FREE_MEMORY;
    }

    /* We have the byte that has a 0 in it, now find where the first 0 is.
     * To check this, we use the fact that an unsigned byte has a 1 in the first bit if
     * and only if it is greater than or equal to pow(2, 8) = 256. */
    for (suffix = 0; suffix < 8; suffix++){
        if (not_byte >= 256) {
            break;
        }
        not_byte <<= 1;
    }

    /* Finally, convert the prefix and suffix into an address. */
    return (prefix << 3) + suffix;
}

/* Set the address addr to empty or full.  Returns errors if the memory is
 * already in the state it is being set to, or if the address is out of
 * bounds.  */
int set_back_addr_empty(short addr) {
    int prefix;
    int suffix;
    byte suffix_bitmask;

    if (addr > BACK_STORE_NUM_FRAME) {
        return ERROR_ADDR_OUT_OF_BOUNDS;
    }

    prefix = addr >> 3;
    suffix = addr & 0x7;
    /* Create the bitmask for the suffix */
    suffix_bitmask = 1 << suffix;

    if (~backing_store_free[prefix] & suffix_bitmask) {
        /* The memory was already set to empty */
        return ERROR_BACKING_EMPTY;
    } else {
        backing_store_free[prefix] = ~(~backing_store_free[prefix] | suffix_bitmask);
        return ERROR_SUCCESS;
    }
}

int set_back_addr_full(short addr) {
    int prefix;
    int suffix;
    byte suffix_bitmask;

    if (addr > BACK_STORE_NUM_FRAME) {
        return ERROR_ADDR_OUT_OF_BOUNDS;
    }

    prefix = addr >> 3;
    suffix = addr & 0x7;
    /* Create the bitmask for the suffix */
    suffix_bitmask = 1 << suffix;

    if (backing_store_free[prefix] & suffix_bitmask) {
        /* The memory was already set to full */
        return ERROR_BACKING_FULL;
    } else {
        backing_store_free[prefix] = backing_store_free[prefix] | suffix_bitmask;
        return ERROR_SUCCESS;
    }
}
