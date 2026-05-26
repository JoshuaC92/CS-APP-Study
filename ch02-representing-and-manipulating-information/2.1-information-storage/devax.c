#include <stdio.h>

int bis(int x; int m){
  return x | m;
}

int bic(int x; int m){
  return x & ~m
}

// bic() is already just a boolean or
int bool_or(int x, int y){
  int result = bis(x,y);
  return result;
}

// we need to find all of the bits that are on in both the mask and the given int
// we can do this by clearing out all the bits that are on in both the mask and the 
// int. turn off all of the bits that are on by reversing the mask and the :wq
//
int bool_xor(int x, int y){
  int result = bis(bic(x,y),bic(y,x));
  return result;
}
