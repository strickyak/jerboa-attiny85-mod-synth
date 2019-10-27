#ifndef JERBOA_MULT_H_
#define JERBOA_MULT_H_

#include "generated-mult-table.h"

// multiply 4 bits of X times 4 bits of Y yielding 8 bits, unsigned.
#define M(X,Y) pgm_read_byte(MULT4X4 + (((X)&15)<<4) + ((Y)&15))

// multiply 8 bits of a times 8 bits of b yielding 16 bits, signed.
int SignedMult8x8(int a, int b) {
  bool neg = false;
  if (a<0) {neg=true; a = -a;}
  if (b<0) {neg=!neg; b = -b;}
  byte p = M(a, b);
  byte q = M((a>>4), b);
  byte r = M(a, (b>>4));
  byte s = M((a>>4), (b>>4));
  word z = word(p) + (word(q)<<4) + (word(r)<<4) + (word(s)<<8);
  return neg ? -z : z;
}

#endif
