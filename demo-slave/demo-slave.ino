#define WHICH_PWM 4
#define LED 3

#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/net1.h"

void Setup() {
  Slave::Setup();
}

void Loop() {
  byte x = Slave::Receive();
  OUT_F(x);
  digitalWrite(3, Slave::slave_toggle ? HIGH : LOW);
}
