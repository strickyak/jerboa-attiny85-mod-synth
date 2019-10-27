#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/random.h"

void Setup() {
	RandomSetup();
}

void Loop() {
  OUT_F(RandomByte());
}
