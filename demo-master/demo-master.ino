#define WHICH_PWM 4
#define LED 3

#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/net1.h"

void Setup() {
  Master::Setup();
}

void Loop() {
  byte i = 0;
  while (1) {
    Master::Send(i);
    OUT_F(i);

    SpinDelay(2);
    ++i;
  }
}
