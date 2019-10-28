#define WHICH_PWM 4

#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/net1.h"

Master m;

void Setup() {
  m.Setup();
}

void Loop() {
  byte i = 0;
  while (1) {
    m.Send(i);
    OUT_F(i);

    SpinDelay(2);
    ++i;
  }
}
