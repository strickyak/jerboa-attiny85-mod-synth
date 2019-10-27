#define MOC_TICKS 0

#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/generated-vfo-table.h"

word phase;

void Setup() {}

void Loop() {
  // LFO differs from VCO by using 0 instead of IN_R().
  word offset = word(IN_A()) + word(IN_B());
  phase += pgm_read_word(VFO_TABLE+offset);

  if (phase & 0x8000) {
    OUT_F((-phase)>>7);
    OUT_F(255);
    LedOn();
  } else {
    OUT_F(phase>>7);
    OUT_F(0);
    LedOff();
  }
}
