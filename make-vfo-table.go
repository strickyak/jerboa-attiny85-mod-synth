// +build main

package main

import (
	"fmt"
	"math"
)

// Span means 256 values, like a single-byte analog input value from A, B, or K.
// If a span is 5V, and that's 5 octaves, we obey the "1V per octave" rule.
// Since there are 3 spans added (A, B, and K) we have a 15-octave range,
// from 8000 Hz down to about 0.25 Hz.
// TODO: our 16-bit arithmetic gets in the way of counting lower than 0.25 Hz; do longer-precision adds?

const RATE = 9615
const MAX_FREQ = 8000
const OCTAVES = 15
const OCTAVES_PER_SPAN = 5
const SPANS = OCTAVES / OCTAVES_PER_SPAN  // == 3
const TINY_STEPS = OCTAVES_PER_SPAN / 256.0
const N = 256 * OCTAVES / OCTAVES_PER_SPAN  // == 768

func main() {
	fmt.Printf("#ifndef VFO_TABLE_H_\n")
	fmt.Printf("#define VFO_TABLE_H_\n\n")

	fmt.Printf("const word VFO_TABLE[] PROGMEM = {\n")

	for i := 1 - N; i <= 0; i++ {
		hz := MAX_FREQ * math.Pow(2, float64(i)*TINY_STEPS)
		fmt.Printf("  /* %4d: %10.4fHz */ %d,\n", i, hz, /*0xFFFF &*/ int(0xFFFF / (RATE/hz)))
	}

	fmt.Printf("};\n")

	fmt.Printf("\n#endif\n")
}
