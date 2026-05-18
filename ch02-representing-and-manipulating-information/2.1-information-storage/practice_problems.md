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


