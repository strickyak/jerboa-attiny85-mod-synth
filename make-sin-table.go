// +build main

package main

import (
	"fmt"
	"math"
)

const N = 256

func main() {
	fmt.Printf("#ifndef SIN_TABLE_H_\n")
	fmt.Printf("#define SIN_TABLE_H_\n\n")

	fmt.Printf("const byte SIN_TABLE[] PROGMEM = {\n")

	for i := 0; i < N; i++ {
		x := float64(i) * math.Pi / 2 / (N-1)
		y := math.Sin(x)
		fmt.Printf("  /* %3d: %10.6f */ 0x%02x,\n", i, y, int(y * 255))
	}

	fmt.Printf("};\n")

	fmt.Printf("\n#endif\n")
}
