/* SLEW:  Copy input A to output F, but with a limit
   on the rate of change, set by K.  If the limit
   is hit, the LED turns on.
*/

#include "/tmp/jerboa/jerboa.h"

byte prev;  // Remember previous F.

void Setup() {
  prev = 128;  // Middle value between 0 and 255.
}

void Loop() {
  // Knob chooses a max delta from 1 to 32.
  int max_delta = 1 + (31 & ((InK() - 32) >>3));

  // no // Knob chooses a max delta from 1 to 255.
  // no int max_delta = InK();
  // no max_delta = (max_delta < 1) ? 1 : max_delta;

  byte a = InA();
  byte f = a;  // for the moment, assume not too fast.
  byte led = 0;  // turn LED on if too fast.

  if (a < prev) {
    // falling
    if (prev - a > max_delta) {
      // falling too fast
      f = prev - max_delta;
      led = 1;
    }
  } else if (a > prev) {
    // rising
    if (a - prev > max_delta) {
      // rising too fast
      f = prev + max_delta;
      led = 1;
    }
  }

  OutF(f);
  prev = f;
  if (led) LedOn(); else LedOff();
}
