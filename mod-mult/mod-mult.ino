#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/mult.h"

void Setup() {}

void Loop() {
  int z = SignedMult8x8(InA()-128, InB()-InK()) >> 6;  // why 6? maybe 7?
  if (z>127) z=127;      // hard clip
  if (z< -127) z= -127;  // hard clip
  OutF(z);
}
