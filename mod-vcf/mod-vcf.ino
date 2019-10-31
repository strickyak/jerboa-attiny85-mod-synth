#include "/tmp/jerboa/jerboa.h"

int filtered;
byte history[256];  // circular buffer
int p;  // points current position in history

void Setup() {}

void Loop() {
  int level = int(InB() >> 3) - int(InK()>>3);
  if (level < 0) level = 0;
  if (level > 31) level = 31;
  byte n = level + 1;

  p = 255 & (p+1);
  byte audio = InA();
  history[p] = audio;

#if 1
  // approximate a convolution of n +1s and n -1s.
  int delta = audio - 2*history[255 & (p-n)] + history[255 & (p-n-n)];
  filtered += delta;
#else
  #define H(I) history[255 & (p-(I))]
  if (n > 8) {
    byte m = n >> 3;
    int delta = audio + 2*H(m) + 4*H(2*m-1) - 4*H(2*m+1) - 2*H(3*m) - 2*H(4*m) - 2*H(5*m) - 4*H(6*m-1) + 4*H(6*m+1) + 2*H(7*m) + H(n);
    filtered += delta;
  } else {
    int delta = audio - 2*history[255 & (p-n)] + history[255 & (p-n-n)];
    filtered += delta;
  }
#endif

  // Clamp filtered output between 0 and 255.
  if (filtered < 0)   filtered = 0;
  if (filtered > 255) filtered = 255;

  OutF(filtered);
}
