# CS:APP 2.1.3 — Addressing and Byte Ordering

## Concept

Every multi-byte object in memory occupies a contiguous sequence of
bytes. Two questions arise:

1. **What address does the object have?** — the address of the *first*
   byte it occupies (lowest address).
2. **In what order are the bytes stored?** — this is **endianness**.

---

## Endianness

Given a 4-byte integer `0x01234567`:

| Byte position (low → high address) | Little-endian (x86/x86-64) | Big-endian (z/Architecture, SPARC) |
|---|---|---|
| addr + 0 | `67` (LSB) | `01` (MSB) |
| addr + 1 | `45` | `23` |
| addr + 2 | `23` | `45` |
| addr + 3 | `01` (MSB) | `67` (LSB) |

**Little-endian**: least significant byte at the lowest address.  
**Big-endian**: most significant byte at the lowest address.

x86/x86-64 (the dominant architecture for exploit development) is
**little-endian**. IBM z/Architecture (mainframe) is **big-endian**.

This matters for exploit development because:
- Shellcode and ROP addresses written to memory must account for byte
  order.
- When crafting a payload like `p64(0xdeadbeef)` in pwntools, the
  library handles the little-endian packing for you.
- When reading a core dump or memory dump from a target, you must
  mentally reverse multi-byte values.

---

## The `byte_pointer` Type

```c
typedef unsigned char *byte_pointer;
```

`unsigned char` is C's canonical byte type:
- Exactly 1 byte (guaranteed by the C standard).
- `unsigned` gives values 0–255 with no sign extension — raw bit
  pattern preserved.

Casting any pointer to `byte_pointer` lets you legally walk the raw
bytes of any object in memory. This is the same technique used in:
- `memcpy` / `memset` implementations
- Exploit primitives that read/write arbitrary memory
- Binary parsers and protocol implementations

---

## `printf("%.2x")`

| Part | Meaning |
|---|---|
| `%` | start format specifier |
| `.2` | minimum 2 digits, zero-padded |
| `x` | lowercase hexadecimal |

Always use `%.2x` (not `%x`) in hex dumps so bytes below `0x10`
print as `07` not `7` — misaligned dumps are hard to read.

---

## `void *` — Generic Pointer

`void *` is C's generic pointer. It accepts any pointer type without
a cast. `show_pointer(void *x)` uses it so the function can receive
a pointer to anything.

Note that `show_pointer` shows the **address itself** as bytes, not
the value at that address. This is how you observe:
- **ASLR**: run the binary twice — the pointer bytes change each time.
- **Pointer leaks**: an info-leak exploit extracts these bytes from
  program output to calculate the base address of code or the heap,
  defeating ASLR.

---

## Files

| File | Purpose |
|---|---|
| `byte_rep.h` | Type definitions and function declarations |
| `byte_rep.c` | Implementation of `show_bytes` and wrappers |
| `main.c` | Test harness — endianness probe, hex dumps, comparisons |
| `Makefile` | Build, run, disassemble, gdb targets |

---

## Build and Run

```bash
make run          # build and execute
make dump         # disassemble full binary (Intel syntax)
make dump-main    # disassemble main() only
make gdb          # load in gdb
make strings      # print strings from binary
make clean        # remove build artifacts
```

---

## ASLR Experiment

Run the binary twice and observe the pointer output changing:

```bash
./byte_rep
./byte_rep
```

To temporarily disable ASLR for repeatable addresses:

```bash
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
./byte_rep   # address is now fixed
./byte_rep   # same address
echo 2 | sudo tee /proc/sys/kernel/randomize_va_space   # re-enable
```

---

## Key Takeaways

- Objects in memory are just sequences of bytes — their interpretation
  depends entirely on the type the code uses to read them.
- Endianness determines the byte order of multi-byte values in memory.
- Casting a pointer to `unsigned char *` is the legal, portable way to
  inspect raw bytes in C.
- The same numeric value encoded as `int` and `float` produces
  completely different byte patterns — the source of type confusion bugs.
