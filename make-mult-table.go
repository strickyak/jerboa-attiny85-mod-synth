// +build main

package main

import (
	"fmt"
)

func main() {
	fmt.Printf("#ifndef MULT_TABLE_H_\n")
	fmt.Printf("#define MULT_TABLE_H_\n\n")

	fmt.Printf("const byte MULT4X4[] PROGMEM = {")
	for i := 0; i < 16; i++ {
		fmt.Printf("\n  /* %3d */ ", i)
		for j := 0; j < 16; j++ {
			fmt.Printf("  %3d,", i*j)
		}
	}
	fmt.Printf("\n};\n")

	fmt.Printf("\n#endif\n")
}
