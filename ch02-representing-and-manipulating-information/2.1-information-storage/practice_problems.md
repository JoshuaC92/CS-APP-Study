# 2.5:
  a = x'12345678'

   *Represented in L (little endian, least significant bits first) and*
    *B (big endian, most significant bits first).*

      A: L-x'78'     B-x'12'
      B: L-x'7856'   B-x'1234'
      C: L-x'785634' B-x'123456'

# 2.6:
  int   x = 2607352    (x'00 27 C8 F8)
  float y = 3510593.0  (x'4A 1F 23 E0)

    A. 
        00 27 C8 F8   0000 0000  0010 0111  1100 1000  1111 1000
        4A 1F 23 E0   0100 1010  0001 1111  0010 0011  1110 0000

    B.
        Shift the int representation 2 bits 
          to the left relative to the float:
         
            00  0000 0000  1001 1111  0010 0011  1110 00 <- 2 bits
                0100 1010  0001 1111  0010 0011  1110 0000
                ^ 
                sign 0 = Positive
                 ^         ^
                 exponent x'94' = 148  
                            ^                            ^
                            mantissa x'1F 23 E0' = 2,040,800 

        21 bits match.
    
    C.  
        The most significant bits do not match.

# 2.7:
    x'6D 6E 6F 70 71 72'

# 2.8:
    0100 1110
    1110 0001

  ~ 1011 0001
  ~ 0001 1110

  & 0100 0000
  | 1110 1111
  ^ 1010 1111

# 2.9:

  A.
      BLACK   ->  WHITE
      BLUE    ->  YELLOW
      GREEN   ->  MAGENTA
      CYAN    ->  RED

  B.
      CYAN
      GREEN
      BLUE

# 2.10:

```c
  void inplace_swap(int *x, int *y){
        *y = *x ^ *y;
        *x = *x ^ *y;
        *y = *x ^ *y;
  }
```

     *x           | *y
    ---------------------
     a            | b
    ---------------------
  1  a            | a ^ b
  2  a ^ (a ^ b)  | a ^ b
  3  a ^ (a ^ b)  | a ^ (a ^ b) ^ (a ^ b)

  In step 2, and given that the ^ (xor) is commutative and associative we can apply the rule that x ^ x = 0, so these steps could be simplified to:

    2. b, a^b
    3. b, a  (the a and b pairs cancel out, leaving us with a lone a)

# 2.11: 

```c
void reverse_array(int a[], int cnt){
    int first,last;
    for (first=0, last=cnt-1;
         first<=last;
         first++,last--)
          inplace_swap(&a[first],&a[last]);
}
```

  A. They will both point to the same array member, and therefore be the same number.
  B. When you xor anything with itself it becomes 0
  C. Add a compare and clause for values a[first] and a[last] and if they are identical leave the values alone.  OR if the array is just assending then change the <= operator to simply <.

# 2.12:
  *See source code compare.c*

# 2.13
  *See sources code devax.c*
