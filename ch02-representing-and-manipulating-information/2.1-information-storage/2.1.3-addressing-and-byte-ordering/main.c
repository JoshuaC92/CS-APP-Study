#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "byte_rep.h"

/*
 * probe_endianness - determine and report byte order of this machine.
 *
 * Stores the integer value 1 and inspects its first byte.
 * On a little-endian machine (x86/x86-64) the least significant byte
 * (0x01) lives at the lowest address, so the first byte is 1.
 * On a big-endian machine (SPARC, some ARM, z/Architecture mainframe)
 * the most significant byte is at the lowest address, so the first
 * byte is 0.
 *
 * Returns: 1 if little-endian, 0 if big-endian.
 *
 * Note for the mainframer: z/Architecture is big-endian. x86-64 is
 * little-endian. This is one of the first things to confirm when
 * analysing a binary from an unknown target platform.
 */
int probe_endianness(void)
{
    int val = 1;
    byte_pointer bp = (byte_pointer) &val;
    return bp[0] == 1;
}

/*
 * hex_dump - print a labelled hex dump of any object in memory.
 * Used throughout the test cases to give context to raw output.
 *
 * This mirrors the kind of output you see from tools like:
 *   xxd, hexdump -C, pwndbg's hexdump command, and GDB's 'x/xb'.
 */
void hex_dump(const char *label, void *addr, size_t len)
{
    printf("  %-20s [ ", label);
    byte_pointer bp = (byte_pointer) addr;
    size_t i;
    for (i = 0; i < len; i++)
        printf("%02x ", bp[i]);
    printf("]\n");
}

/* ------------------------------------------------------------------ */

void test_endianness(void)
{
    printf("=== Endianness Probe ===\n");
    int little = probe_endianness();
    printf("  This machine is: %s-endian\n",
           little ? "LITTLE" : "BIG");
    printf("  (x86/x86-64 = little | z/Architecture mainframe = big)\n\n");

    /*
     * Show the proof: integer 1 laid out in memory.
     * Little-endian:  01 00 00 00
     * Big-endian:     00 00 00 01
     */
    int val = 1;
    printf("  int val = 1, raw bytes in memory order:\n");
    hex_dump("val", &val, sizeof(int));
    printf("\n");
}

/* ------------------------------------------------------------------ */

void test_show_int(void)
{
    printf("=== show_int ===\n");

    /*
     * 12345 in hex is 0x00003039.
     * On a little-endian machine the bytes appear reversed in memory:
     *   39 30 00 00
     * The least significant byte (0x39) is stored at the lowest address.
     *
     * This byte-reversal is one of the first things that catches people
     * out when reading a hex dump or a core file from an x86 target.
     */
    int a = 12345;
    printf("  show_int(12345)    -> expected on LE: 39 30 00 00\n");
    printf("  actual:            ->");
    show_int(a);

    /*
     * -1 in two's complement is all bits set: 0xFFFFFFFF.
     * Every byte is 0xff regardless of endianness.
     * Useful sanity check -- if you don't see ff ff ff ff, something
     * is wrong with the cast or the sizeof.
     */
    int b = -1;
    printf("  show_int(-1)       -> expected:       ff ff ff ff\n");
    printf("  actual:            ->");
    show_int(b);

    /*
     * 0 is all bits clear.
     */
    int c = 0;
    printf("  show_int(0)        -> expected:       00 00 00 00\n");
    printf("  actual:            ->");
    show_int(c);

    printf("\n");
}

/* ------------------------------------------------------------------ */

void test_show_float(void)
{
    printf("=== show_float ===\n");

    /*
     * 12345.0f in IEEE 754 single precision is 0x4640E400.
     * On a little-endian machine: 00 e4 40 46
     *
     * Compare this to show_int(12345) which gave: 39 30 00 00
     * Same *number*, completely different bit pattern -- because int
     * and float use entirely different encoding schemes.
     *
     * This difference is the foundation of type confusion bugs:
     * if code accidentally reinterprets float bytes as an int or vice
     * versa, the value it operates on is completely wrong.
     */
    float a = 12345.0f;
    printf("  show_float(12345.0)-> expected on LE: 00 e4 40 46\n");
    printf("  actual:            ->");
    show_float(a);

    /*
     * 0.0f is all bits zero in IEEE 754. Same as integer 0 in this
     * case, but that is a coincidence of the encoding, not a rule.
     */
    float b = 0.0f;
    printf("  show_float(0.0)    -> expected:       00 00 00 00\n");
    printf("  actual:            ->");
    show_float(b);

    /*
     * -0.0f: IEEE 754 has a negative zero. The sign bit is 1, all
     * others are 0: 0x80000000. On little-endian: 00 00 00 80.
     * This surprises most people and is a source of subtle bugs.
     */
    float c = -0.0f;
    printf("  show_float(-0.0)   -> expected on LE: 00 00 00 80\n");
    printf("  actual:            ->");
    show_float(c);

    printf("\n");
}

/* ------------------------------------------------------------------ */

void test_show_pointer(void)
{
    printf("=== show_pointer ===\n");

    /*
     * We pass the address of a local variable.
     * The exact bytes will differ every run if ASLR is enabled
     * (which it is by default on Linux).
     *
     * Run this program twice and compare the pointer output.
     * If ASLR is working, the addresses will be different each time.
     *
     * To disable ASLR temporarily for repeatable results:
     *   echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
     * To re-enable:
     *   echo 2 | sudo tee /proc/sys/kernel/randomize_va_space
     *
     * Recovering these bytes from a program's output is what an
     * "information leak" or "pointer leak" exploit does -- it reads
     * the raw pointer bytes to calculate where code or data lives,
     * defeating ASLR.
     */
    int val = 42;
    printf("  Address of local int val = 42:\n");
    printf("  show_pointer(&val) -> ");
    show_pointer(&val);

    /*
     * Print the address via printf %p as well so you can cross-check.
     * %p prints in the platform's native pointer format (hex with 0x).
     * The bytes shown by show_pointer should match %p read right-to-left
     * on a little-endian machine.
     */
    printf("  printf(%%p)         -> %p\n", (void *)&val);
    printf("  (read show_pointer right-to-left to match %%p on LE)\n\n");
}

/* ------------------------------------------------------------------ */

void test_int_float_overlap(void)
{
    /*
     * CS:APP explicitly points out that 12345 as int and 12345.0 as
     * float share some byte patterns by coincidence of their encodings.
     * This test makes that comparison explicit and side by side.
     *
     * This kind of deliberate comparison is what you do when analysing
     * a binary to understand how the compiler has encoded data, or when
     * looking for type confusion primitives in a target.
     */
    printf("=== int vs float byte comparison (CS:APP 2.1.3) ===\n");

    int   i = 12345;
    float f = 12345.0f;

    printf("  int   12345  : ");
    show_int(i);
    printf("  float 12345.0: ");
    show_float(f);
    printf("  (same value, different encoding -- this is the point)\n\n");
}

/* ------------------------------------------------------------------ */

int main(void)
{
    printf("\n");
    printf("========================================================\n");
    printf("  CS:APP 2.1.3 -- Byte Representation of Program Data  \n");
    printf("========================================================\n\n");

    test_endianness();
    test_show_int();
    test_show_float();
    test_show_pointer();
    test_int_float_overlap();

    printf("========================================================\n");
    printf("  To inspect further, run:\n");
    printf("    make dump    -- disassemble the binary with objdump\n");
    printf("    make gdb     -- load the binary in gdb\n");
    printf("========================================================\n\n");

    return 0;
}
