#include <stdio.h>
#include "byte_rep.h"

/*
 * show_bytes - dumps a region of memory as hex bytes.
 *
 * Parameters:
 *   start  - byte_pointer (unsigned char *) to the first byte to read.
 *            In each of the wrapper functions below, this will be the
 *            address of a local variable cast to byte_pointer.
 *   len    - number of bytes to read, supplied by sizeof() in the
 *            wrappers so it is always correct for the type.
 *
 * How it works:
 *   The loop increments i from 0 to len-1. Each iteration, start[i]
 *   is pointer arithmetic: it reads the byte at address (start + i),
 *   i.e. it walks forward one byte at a time through the object's
 *   memory representation.
 *
 * printf format "%.2x":
 *   %    - begin format specifier
 *   .2   - minimum 2 digits, left-padded with zeros if needed
 *   x    - print as lowercase hexadecimal
 *
 *   Examples: value 7   -> "07"
 *             value 160 -> "a0"
 *             value 255 -> "ff"
 *
 *   The zero-padding is important: without it, bytes below 0x10
 *   print as a single character, misaligning the dump and making
 *   it harder to read -- critical when you are eyeballing memory
 *   layouts for exploitation.
 */
void show_bytes(byte_pointer start, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++)
        printf(" %.2x", start[i]);
    printf("\n");
}

/*
 * show_int - show the raw byte representation of an int.
 *
 * &x          gives us the address of x in memory.
 * (byte_pointer) casts that address from (int *) to (unsigned char *),
 *              so show_bytes can legally walk the bytes one at a time.
 * sizeof(int)  is typically 4 bytes on a 64-bit system, but is not
 *              guaranteed -- sizeof lets the compiler tell us the truth
 *              rather than us hardcoding 4.
 *
 * The byte order you observe in the output will reflect your machine's
 * endianness. On x86/x86-64 (little-endian) the least significant byte
 * appears first (lowest address).
 */
void show_int(int x)
{
    show_bytes((byte_pointer) &x, sizeof(int));
}

/*
 * show_float - show the raw byte representation of a float.
 *
 * Identical pattern to show_int. A float is also 4 bytes (IEEE 754
 * single precision). The interesting thing here is that an int and a
 * float with the same numeric value will have completely different byte
 * patterns -- they use entirely different encoding schemes.
 *
 * This is directly relevant to type confusion bugs and exploit
 * primitives that rely on reinterpreting the bytes of one type as
 * another (e.g. int/float confusion in JIT compilers).
 */
void show_float(float x)
{
    show_bytes((byte_pointer) &x, sizeof(float));
}

/*
 * show_pointer - show the raw byte representation of a pointer (address).
 *
 * Parameter is void * -- C's generic pointer type.
 * void * can receive any pointer type without a cast, making this
 * wrapper usable for pointers to any type.
 *
 * Note: we are NOT showing the value at the address.
 *       We are showing the address itself as raw bytes.
 *       On a 64-bit system sizeof(void *) is 8 bytes.
 *
 * This is how you observe things like:
 *   - ASLR: run the program twice and watch the address bytes change.
 *   - Pointer leaks: an info-leak exploit recovers these bytes from
 *     program output to defeat ASLR and calculate base addresses.
 *
 * &x here gives us the address of the local copy of the pointer x,
 * i.e. the address of an address -- a pointer to a pointer.
 * sizeof(void *) gives us the size of a pointer on this platform.
 */
void show_pointer(void *x)
{
    show_bytes((byte_pointer) &x, sizeof(void *));
}
