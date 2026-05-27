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
  signed char as = 0xD4;
  int b = 0x64;
  signed char bs = 0x64;
  int c = 0x72;
  signed char cs = 0x72;
  int d = 0x44;
  signed char ds = 0x44;

           int als = a<<2;
  unsigned int arl = as>>3;
    signed int ara = a>>3;

         int bls = b<<2;
  unsigned int brl = bs>>3;
    signed int bra = b>>3;

         int cls = c<<2;
  unsigned int crl = cs>>3;
    signed int cra = c>>3;

         int dls = d<<2;
  unsigned int drl = ds>>3;
    signed int dra = d>>3;


  printf("    A Before: ");
  show_bytes((byte_pointer) &a, sizeof(int));
  show_bytes((byte_pointer) &als, sizeof(int));
  show_bytes((byte_pointer) &arl, sizeof(int));
  show_bytes((byte_pointer) &ara, sizeof(int));

  printf("    B Before: ");
  show_bytes((byte_pointer) &b, sizeof(int));
  show_bytes((byte_pointer) &bls, sizeof(int));
  show_bytes((byte_pointer) &brl, sizeof(int));
  show_bytes((byte_pointer) &bra, sizeof(int));

  printf("    C Before: ");
  show_bytes((byte_pointer) &c, sizeof(int));
  show_bytes((byte_pointer) &cls, sizeof(int));
  show_bytes((byte_pointer) &crl, sizeof(int));
  show_bytes((byte_pointer) &cra, sizeof(int));

  printf("    D Before: ");
  show_bytes((byte_pointer) &d, sizeof(int));
  show_bytes((byte_pointer) &dls, sizeof(int));
  show_bytes((byte_pointer) &drl, sizeof(int));
  show_bytes((byte_pointer) &dra, sizeof(int));

}
