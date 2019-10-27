 // mod-envelope.ino
#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/mult.h"

// ENVELOPE:
// A: audio signal
// B: trigger (triggers at 75%)
// Not yet implemented: R: {attack,decay,release} speed

byte mode;
word ticks;
int level;  // 0 to 127

void Setup() {}

#define RAMP 31 // 127 /* 2**N - 1 */

void Loop() {
  byte a = IN_A();
  byte b = IN_B();
  bool high = (b > 0xC0);

  ++ticks;
  if (mode) {
    if (high) {
      if (level < 127 && (ticks&RAMP)==0) {
        ++level;
      }
    } else {
      //ticks = 0;
      mode = false;
    }
  } else {
    if (high) {
      //ticks = 0;
      mode = true;
    } else {
      if (level > 0 && (ticks&RAMP)==0) {
        --level;
      }
    }
  }
  
  int z = SignedMult8x8(a-128, level) >> 6;
  if (z>127) z=127;
  if (z< -127) z= -127;
  OUT_F(128 + z);

  if (high) LedOn(); else LedOff();
}
