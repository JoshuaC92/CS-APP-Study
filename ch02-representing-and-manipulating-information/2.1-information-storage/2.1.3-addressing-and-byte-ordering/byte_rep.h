#ifndef BYTE_REP_H
#define BYTE_REP_H

#include <stddef.h>

/*
 * byte_pointer - a pointer to a single unsigned byte in memory.
 *
 * Using 'unsigned char' is deliberate:
 *   - char is guaranteed to be exactly 1 byte in C (the smallest
 *     addressable unit of memory).
 *   - unsigned ensures values are interpreted as 0-255 with no sign
 *     bit, giving us the raw bit pattern of each byte without any
 *     sign extension distorting the output.
 *
 * This is the canonical type used in C for byte-level memory access,
 * and is the same approach used in low-level systems and exploit code
 * when inspecting raw memory.
 */
typedef unsigned char *byte_pointer;

/*
 * show_bytes - print len bytes of memory starting at address start.
 * Each byte is printed as a zero-padded 2-digit hex value.
 */
void show_bytes(byte_pointer start, size_t len);

/*
 * Type-specific wrappers around show_bytes.
 * Each casts the address of x to a byte_pointer and passes the
 * correct size for the type, letting show_bytes walk the raw bytes.
 */
void show_int(int x);
void show_float(float x);
void show_pointer(void *x);

#endif /* BYTE_REP_H */
