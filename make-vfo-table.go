// +build main

package main

import (
	"fmt"
	"math"
)

// Span means 256 values, like a single-byte analog input value.

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
