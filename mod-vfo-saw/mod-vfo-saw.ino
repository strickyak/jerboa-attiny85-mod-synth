#define MOC_TICKS 0

#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/generated-vfo-table.h"

word phase;

void Setup() {}

void Loop() {
  word offset = word(IN_A()) + word(IN_B()) + word(IN_R());
  phase += pgm_read_word(VFO_TABLE+offset);
  OUT_F(phase>>8);

  if (phase & 0x8000) {
    OUT_F(255);
    LedOn();
  } else {
    OUT_F(0);
    LedOff();
  }
}
