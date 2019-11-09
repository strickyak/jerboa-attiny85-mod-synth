#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/random.h"

// Make sure it can run max speed even if the Knob doesn't go down to 0.
// It just needs to go down to 32.
constexpr word INITIAL = 32;

word wait;

void Setup() {
  RandomSetup();
  wait = INITIAL;
}

void Loop() {
  ++wait;
  if (wait < InK()) return;
  wait = INITIAL;
  byte b = RandomByte();
  OutF(b);
  if (b>240) LedOn(); else LedOff();
}
