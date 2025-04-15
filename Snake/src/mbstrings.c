#include "mbstrings.h"
#include <stdint.h>

/* mbslen - multi-byte string length
 * - Description: returns the number of UTF-8 code points ("characters")
 * in a multibyte string. If the argument is NULL or an invalid UTF-8
 * string is passed, returns -1.
 *
 * - Arguments: A pointer to a character array (`bytes`), consisting of UTF-8
 * variable-length encoded multibyte code points.
 *
 * - Return: returns the actual number of UTF-8 code points in `src`. If an
 * invalid sequence of bytes is encountered, return -1.
 *
 * - Hints:
 * UTF-8 characters are encoded in 1 to 4 bytes. The number of leading 1s in the
 * highest order byte indicates the length (in bytes) of the character. For
 * example, a character with the encoding 1111.... is 4 bytes long, a character
 * with the encoding 1110.... is 3 bytes long, and a character with the encoding
 * 1100.... is 2 bytes long. Single-byte UTF-8 characters were designed to be
 * compatible with ASCII. As such, the first bit of a 1-byte UTF-8 character is
 * 0.......
 *
 * You will need bitwise operations for this part of the assignment!
 */

size_t num_of_bytes(unsigned char firstByte){
    //remove index 0-3 of first4bytes
    unsigned char firstFourBits = firstByte >> 4;

    switch(firstFourBits){
        case 0b0000 ... 0b0111:
            return 1;
        case 0b1100:
        case 0b1101:
            return 2;
        case 0b1110:
            return 3;
        case 0b1111:
            return 4;
        default:
            return -1;

    }
}



size_t mbslen(const char* bytes) {
    // TODO: implement!
    int index = 0;

    int char_count = 0;
    while(bytes[index] != '\0'){
        //grab byte with index
        unsigned char first_byte = bytes[index];
        //get length of byte
        size_t byte_num = num_of_bytes(first_byte);

        //jump index to next byte
        index += byte_num;
        char_count++;

    }

    return char_count - 1;

}
