#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/generated-vco-table.h"
#include "/tmp/jerboa/generated-sin-table.h"

word phase;

void Setup() {}

void Loop() {
  word offset = word(InA()) + word(InB()) + word(InK());
  phase += pgm_read_word(VCO_TABLE+offset);

  byte x = 0xFF & (phase >> 6);
  byte y;
  switch (phase & 0xC000) {
  case 0x0000:
    y = pgm_read_byte(SIN_TABLE + x);
    OutF(128 + (y>>1));
    LedOn();
    break;
  case 0x4000:
    y = pgm_read_byte(SIN_TABLE + 255 - x);
    OutF(128 + (y>>1));
    LedOn();
    break;
  case 0x8000:
    y = pgm_read_byte(SIN_TABLE + x);
    OutF(128 - (y>>1));
    LedOff();
    break;
  case 0xC000:
    y = pgm_read_byte(SIN_TABLE + 255 - x);
    OutF(128 - (y>>1));
    LedOff();
    break;
  }
}
