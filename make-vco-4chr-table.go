// +build main

package main

/*
  Say we want 4 octaves, from loC--C--midC-(A440)-C--hiC.
  That's 4*12+1 = 49 notes (includes C at both ends).
*/

import (
	"fmt"
	"log"
	"math"
)

// Span means 256 values, like a single-byte analog input value from A, B, or K.
// If a span is 5V, and that's 5 octaves, we obey the "1V per octave" rule.
// Since there are 3 spans added (A, B, and K) we have a 15-octave range,
// from 8000 Hz down to about 0.25 Hz.
// TODO: our 16-bit arithmetic gets in the way of counting lower than 0.25 Hz; do longer-precision adds?

const RATE = 9615 * 2

// const MAX_FREQ = 8000
// const OCTAVES = 15
// const OCTAVES_PER_SPAN = 5
// const SPANS = OCTAVES / OCTAVES_PER_SPAN  // == 3
// const TINY_STEPS = OCTAVES_PER_SPAN / 256.0

func main() {
	fmt.Printf("#ifndef VCO_4CHR_TABLE_H_\n")
	fmt.Printf("#define VCO_4CHR_TABLE_H_\n\n")

	fmt.Printf("const word VCO_4CHR_TABLE[] PROGMEM = {\n")

	// From A440, 3 steps to C, and 12 more to High C.
	MaxFreq := 440.0 * math.Pow(2, (3+12)/12.0)
	MinFreq := MaxFreq / 16
	log.Printf("MinFreq %f MaxFreq %f", MinFreq, MaxFreq)

	for i := 0; i < 256; i++ {
		note := (i - 5) / 5
		if note < 0 {
			note = 0
		}
		if note > 48 {
			note = 48
		}

		hz := MinFreq * math.Pow(2, float64(note)/12.0)
		log.Printf("i=%d note=%d hz=%f", i, note, hz)

		fmt.Printf("  /* %4d: %10.4fHz */ %d,\n", i, hz /*0xFFFF &*/, int(0x10000/(RATE/hz)))
	}

	fmt.Printf("};\n")

	fmt.Printf("\n#endif\n")
}
