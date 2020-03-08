#include "/tmp/jerboa/jerboa.h"

byte H[256];  // circular buffer for History.
byte p;  // points current position in H

void Setup() {}

void Loop() {
  int level = int(InB() >> 3) - int(InK()>>3);
  if (level < 0) level = 0;
  if (level > 31) level = 31;
  byte n = level + 1;  // n==0 just outputs zero.

  ++p;
  byte audio = InA();
  H[p] = audio;

  byte p1 = p-n;
  byte p2 = p1-n;
  byte p3 = p2-n;
  byte p4 = p3-n;
  byte p5 = p4-n;
  byte p6 = p5-n;
  byte p7 = p6-n;

  // Do the convlution with four +1 terms and four -1 terms, separated by zeros.
  // Improvements: multiply by 2 instead of 1.
  // Cast first term to (unsigned) to defeat byte arithmetic in cmoc-like compilers.
  int out = (int)((unsigned)audio + 2*H[p1] + 2*H[p2] + H[p3]);
  out -= (int)((unsigned)H[p4] + 2*H[p5] + 2*H[p6] + H[p7]);

  // Clamp filtered output between 0 and 255.
  if (out < 0)   out = 0;
  if (out > 255) out = 255;
  OutF(out);
}
