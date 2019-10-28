#include "/tmp/jerboa/jerboa.h"

constexpr int N = 450;
byte mem[N];
int p;

void Setup() {}

void Loop() {
  word a = IN_A();
  word mix = (a + mem[p]) >> 1;

  mem[p] = mix;
  ++p;
  if (p==N) p=0;

  OUT_F(mix);
}
