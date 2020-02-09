#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/generated-vco-table.h"

word phase;

void Setup() {}

void Loop() {
  word offset = word(InA()) + word(InB()) + word(InK());
  phase += pgm_read_word(VCO_TABLE+offset);
  
  OutF(phase>>8);

  if (phase & 0x8000) {
    LedOn();
  } else {
    LedOff();
  }
}
