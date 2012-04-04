#include <stdio.h>

/**
 Convert an int n to a binary string.  str should be a char[sizeof(int)*8 + 1]
*/

void itobin(int n, char *str) {
    int place = 0;
    int i;
    const int bytes_in_int = 8*sizeof(int);

    for (i = 0; i < 8*sizeof(int); i++){
        if (n & 1){
            str[bytes_in_int - 1 - place] = '1';
        } else {
            str[bytes_in_int - 1 - place] = '0';
        }
        n >>= 1;
        place++;
    }
    str[8*sizeof(int)] = '\0';
}
