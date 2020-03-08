// mod-2qx-vca is a 2-quadrant multiplier (Voltage Controlled Amplifier).

#include "/tmp/jerboa/jerboa.h"

void Setup() {}

void Loop() {
  byte audio = InA();
  int a = (int)audio - 128;
  int b = int(InB()) - int(InK()) - 8 /*easier to zero*/;
  if (b<0) b=0;

  int z = (a * b) >> 8;

  if (z>127) z=127;      // hard clip
  if (z< -127) z= -127;  // hard clip
  OutF(z+128);
  if (z<=128) LedOff(); else LedOn();
}
