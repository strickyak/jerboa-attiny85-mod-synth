#define WHICH_PWM 4
#define WHICH_LED 3

#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/net1.h"

void Setup() {
  Slave::Setup();
}

void Loop() {
  byte x = Slave::Receive();
  OUT_F(x);
  
  if (x==31) {
    digitalWrite(WHICH_LED, LOW);
  } else if (x==170) {
    digitalWrite(WHICH_LED, HIGH);
  }
  
  // digitalWrite(3, Slave::slave_toggle ? HIGH : LOW);
}
