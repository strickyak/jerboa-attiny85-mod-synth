// +build main

package main

import (
	"fmt"
)

// "4chr" encodes 49 notes as voltage levels with snap-to-closest chromatic scale note.
// The notes are from C2 to C6 (4 octaves plus an extra C) and these are represented
// by voltage levels 7 to 247, stepping by 5 levels for each chromatic half-step.
//
// By voltage level we mean the PCM output `OutF(voltLevel)`, so levels 0 to 255
// correspond to a range of (roughly) 0 to +5V.
//
// Midi tone numbers run from 1 to 127, representing chromatic half-steps,
// with 36 for C2, 60 for C4, and 84 for C6.  Since we only use 36 to 84,
// mapping values below 36 or above 84 into the bottom or top octave that we support
// seems the kindest thing we can do.

func main() {
	fmt.Printf("#ifndef MIDI4CHR_TABLE_H_\n")
	fmt.Printf("#define MIDI4CHR_TABLE_H_\n\n")

	fmt.Printf("const byte MIDI4CHR_TABLE[] PROGMEM = {\n")

	for i := 0; i < 128; i++ {
		j := i
		for j < 36 {
			j += 12
		}
		for j > 84 {
			j -= 12
		}
		voltLevel := 5*(j-36) + 7 // Map tones to volt levels.
		fmt.Printf("  /* midi %3d -> %3d: */ %3d,\n", i, j, voltLevel)
	}

	fmt.Printf("};\n")

	fmt.Printf("\n#endif\n")
}
