#include <stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len){
  int i;
  for (i = 0; i < len; i++)
    printf(" %.2x",start[i]);
  printf("\n");
}

void inplace_swap(int *x, int *y){
  *y = *x ^ *y;
  *x = *x ^ *y;
  *y = *x ^ *y;
}

void rev_array(int a[], int cnt){
  int first, last;
  for (first = 0, last = cnt-1;
       first <= last;
       first++, last--)
    inplace_swap(&a[first], &a[last]);
}

void main(){
  int x = 0x87654321;
  
  int a = x & 0xFF;
  int b = x ^ ~0xFF;
  int c = x | 0xFF;
  
  // byte order will be printed in reverse because of little endian-ness
  printf(" Before: ");
  show_bytes((byte_pointer) &x, sizeof(int));

  printf("A After: ");
  show_bytes((byte_pointer) &a, sizeof(int));
  
  printf("B After: ");
  show_bytes((byte_pointer) &b, sizeof(int));

  printf("C After: ");
  show_bytes((byte_pointer) &c, sizeof(int));
}
