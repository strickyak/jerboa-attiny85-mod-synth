// mod-hold
#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/generated-vfo-table.h"

void Setup() {}

void Loop() {
  bool gate = word(InB()) + word(InK()) >= 0xE0;
  if (gate) {
    // Time for a new sample.
    OutF(InA());
    LedOff();
  } else {
    LedOn();  // LED indicates holding
  }
}
