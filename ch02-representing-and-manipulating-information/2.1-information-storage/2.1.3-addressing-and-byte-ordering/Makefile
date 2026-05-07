# =============================================================
# CS:APP 2.1.3 -- Byte Representation
# =============================================================
#
# Security-oriented build flags:
#   -g                  include DWARF debug info for gdb
#   -fno-stack-protector disable stack canaries (relevant later
#                       for stack exploitation chapters)
#   -no-pie             disable position-independent executable
#                       so addresses are fixed (easier to follow
#                       in early study; we'll re-enable for ASLR
#                       chapters)
#   -Wall -Wextra       catch sloppy code early
#   -m32 / (default)    uncomment -m32 to build 32-bit and
#                       observe how pointer size changes
#
# =============================================================

CC      = gcc
CFLAGS  = -g -Wall -Wextra -fno-stack-protector -no-pie
TARGET  = byte_rep
SRCS    = main.c byte_rep.c
OBJS    = $(SRCS:.c=.o)

# -----------------------------------------------------------
# Default target: build the binary
# -----------------------------------------------------------
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c byte_rep.h
	$(CC) $(CFLAGS) -c -o $@ $<

# -----------------------------------------------------------
# run -- build and execute
# -----------------------------------------------------------
run: all
	./$(TARGET)

# -----------------------------------------------------------
# dump -- disassemble with objdump
# Shows the compiled machine code alongside source line info.
# This is one of the primary tools for static binary analysis.
# -d   disassemble executable sections
# -S   interleave source (requires -g in CFLAGS)
# -M intel  use Intel syntax (standard in x86 RE / exploit dev)
# -----------------------------------------------------------
dump: all
	objdump -d -S -M intel $(TARGET)

# -----------------------------------------------------------
# dump-main -- disassemble only the main function
# Useful for focusing on a specific function without noise.
# -----------------------------------------------------------
dump-main: all
	objdump -d -S -M intel $(TARGET) | awk '/^[0-9a-f]+ <main>/,/^[0-9a-f]+ </'

# -----------------------------------------------------------
# gdb -- load binary in gdb
# If you have pwndbg or peda installed they will load
# automatically via your ~/.gdbinit.
# Suggested starting commands inside gdb:
#   break main
#   run
#   x/4xb &val    <- examine 4 bytes at address of val
#   p/x val       <- print val in hex
# -----------------------------------------------------------
gdb: all
	gdb ./$(TARGET)

# -----------------------------------------------------------
# strings -- print printable strings from the binary
# Useful habit: always run strings on an unknown binary first.
# -----------------------------------------------------------
strings: all
	strings ./$(TARGET)

# -----------------------------------------------------------
# clean -- remove build artifacts
# -----------------------------------------------------------
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all run dump dump-main gdb strings clean
