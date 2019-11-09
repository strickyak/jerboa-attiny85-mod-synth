#include "/tmp/jerboa/jerboa.h"

constexpr int N = 450;
byte mem[N];
int p;
byte stretch;

void Setup() {}

void Loop() {
  int level = 31 & ((InK() - 32) >>3);
  byte n = level + 1;

  ++stretch;
  if (stretch < n) {
    return;
  }
  stretch = 0;
  
  word a = InA();
  word mix = (a + mem[p]) >> 1;

  mem[p] = mix;
  ++p;
  if (p==N) p=0;

  OutF(mix);
}
