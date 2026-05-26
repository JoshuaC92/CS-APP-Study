#include <stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len){
  int i;
  for (i = 0; i < len; i++)
    printf(" %.2x",start[i]);
  printf("\n");
}

void main(){
  
  int a = 0xD4;
  int b = 0x64;
  int c = 0x72;
  int d = 0x44;

           int als = a<<2;
  unsigned int arl = a>>3;
    signed int ara = a>>3;
  
  // byte order will be printed in reverse because of little endian-ness
  printf("    A Before: ");
  show_bytes((byte_pointer) &a, sizeof(int));
  printf("      A left: ");
  show_bytes((byte_pointer) &als, sizeof(int));
  printf(" A right log:");
  show_bytes((byte_pointer) &arl, sizeof(int));
  printf("A right Arit:");
  show_bytes((byte_pointer) &ara, sizeof(int));
}
