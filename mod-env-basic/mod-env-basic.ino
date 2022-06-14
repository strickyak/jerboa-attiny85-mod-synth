// Very simple envelope.
// 50ms attack, 100ms decay, 1/4 amplitude sustain, 100ms release.

#include "/tmp/jerboa/jerboa.h"

byte state;
word timer;

void Setup() {
}

void Loop() {
  byte f;
  timer++;
  bool gate = (InB() > 128);

  if (gate == state) {
    if (gate) {
      if (timer < 1024) {
        f = (byte)(timer >> 2);
      } else if (timer < 3*1024) {
        f = 127 & ((timer-1024) >> 4);
        f = 255 - f - (f>>1);
      } else {
        timer = 30000;
        f = 64;
      }
    } else {
      if (timer < 2048) {
        f = 63 & (timer >> 6);
        f = 64 - f;
      } else {
        timer = 30000;
        f = 0;
      }
    }
    OutF(f);
  } else {
    state = gate;
    timer = 0;
    if(state) LedOn(); else LedOff();
  }
}
