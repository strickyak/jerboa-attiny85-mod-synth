// Emit control voltages (one volt per octave)
// playing the Corale Tune to Jesu, Joy of Man's Desiring,
// by J. S. Bach.

// In Knob: tempo (should be a low voltage to sound reasonable).
// Out F: tone control voltage.
// Out L: Led can be used as an envelope gate.

#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/generated-jesu-table.h"

word phase;  // 0 to 0xFFFF; tone starts at 0, ends at 0xC000
int n;
int i;

void Setup() {
  n = sizeof(JESU_TABLE) / sizeof(byte);
}

void Loop() {
  word prev = phase;
  phase += 1 + word(InK()>>1);  // Use knob to adjust tempo.

  if (phase < prev) {  // If around the corner
    LedOn();

    // Output control voltage for choosing note.
    byte f = pgm_read_byte(JESU_TABLE + i);
    OutF(f);

    i++;  // Advance to next note.
    if (i >= n) i = 0;
  } else if (phase >= 0xC000) {
    LedOff();
  }
}
