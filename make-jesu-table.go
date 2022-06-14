package main

import "fmt"

const LOWEST_TONE = 12

const (
	C = iota
	Cs
	D
	Ds
	E
	F
	Fs
	G
	Gs
	A
	As
	B
	C2
	Cs2
	D2
	Ds2
	E2
	F2
	Fs2
	G2
	Gs2
	A2
	As2
	B2
	C3
)

var Jesu = []int{
	/*rest,*/ G, A, B, D2, C2, C2, E2, D2,
	D2, G2, Fs2, G2, D2, B, G, A, B,
	C2, D2, E2, D2, C2, B, A, B, G,
	Fs, G, A, D, Fs, A, C2, B, A,
	B, G, A, B, D2, C2, C2, E2, D2,
	D2, G2, Fs2, G2, D2, B, G, A, B,
	E, D2, C2, B, A, G, D, G, Fs,
	G, B, D2, G2, D2, B, G, B, D2,
	G2,
}

func main() {
	fmt.Printf(`
#ifndef JESU_TABLE_H_
#define JESU_TABLE_H_

const byte JESU_TABLE[] PROGMEM = {
`)

	for i, note := range Jesu {
		const VOLTAGE_STEPS = 256
		const OCTAVE = 12
		volt := int(float64(note+LOWEST_TONE)*VOLTAGE_STEPS/(5*OCTAVE) + 0.5)
		fmt.Printf("    %d, // [%4d : %2d]\n", volt, i, note+LOWEST_TONE)
	}

	fmt.Printf(`
};
#endif
`)
}
