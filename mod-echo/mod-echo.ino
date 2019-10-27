#include "/tmp/jerboa/jerboa.h"
#include "/tmp/jerboa/generated-vfo-table.h"

constexpr int N = 450;
byte mem[N];
int p;

void Setup() {}

void Loop() {
  word a = IN_A();
  word b = IN_B();

  word c = (a + a + a + b + b + b + mem[p] + mem[p]) >> 3;

  mem[p] = c;
  OUT_F(c);
  ++p;
  if (p==N) p=0;
}
