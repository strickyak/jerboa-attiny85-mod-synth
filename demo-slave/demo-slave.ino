#define WHICH_PWM 4
#define LED 3

#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/net1.h"

Slave s;

void Setup() {
  s.Setup();
}

void Loop() {
  while (true) {
    //Fault(4);
    byte x = s.Receive();
    Fault(5);
    OUT_F(x);
    Fault(1+(x&15));
  }
}
