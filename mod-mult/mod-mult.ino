#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/mult.h"

void Setup() {}

void Loop() {
  int z = SignedMult8x8(IN_A()-128, IN_B()-128) >> 6;
  if (z>127) z=127;
  if (z< -127) z= -127;
  OUT_F(128 + z);
}
