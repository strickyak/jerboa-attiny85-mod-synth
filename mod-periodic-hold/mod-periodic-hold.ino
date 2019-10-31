// mod-hold

#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/generated-vfo-table.h"

word phase;

void Setup() {}

void Loop() {
  word level = word(InB()) + word(InK()) + 0;
  word delta = pgm_read_word(VFO_TABLE+level);
  word next = phase + delta;
  if (next < phase) {
    // We wrapped backwards.  Time for a new sample.
    OutF(InA());
    LedToggle();  // Flash at half the sample rate.
  }
  phase = next;
}
