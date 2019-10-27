#ifndef JERBOA_H_
#define JERBOA_H_

#ifndef MOC_EXTRA_LOOP_COUNT
#define MOC_EXTRA_LOOP_COUNT 0
#endif

#ifndef MOC_TICKS
#define MOC_TICKS 0
#endif


// ATtiny{25,45,85}
//
//         +--u--+
//       x |R   V| x
//   (3) 3 |A   D| 2 (1)
//   (2) 4 |B   F| 1
//       x |G   C| 0
//         +-----+
//  (ADC)          PB

extern void Setup(void);
extern void Loop(void);

namespace gerboa_internal {

volatile word spin_tmp;
void SpinDelay(word n) {
  for (word i = 0; i < n; i++) {
    for (word j = 0; j < 100; j++) {
      spin_tmp += j;
    }
  }
}

const PROGMEM char MoctalTick_GapFollows[] = {0/*unused*/, 0,1, 0,0,1, 0,0,1};

bool led;
void LedOn() { led = true; PORTB = 0x3F; }    // Set low bit; other bits are pullups.
void LedOff() { led = false; PORTB = 0x3E; }  // Clear low bit; other bits are pullups.
void LedToggle() { if (led) LedOff(); else LedOn(); }

// Fault(n) stops everything else and makes flashy pulses in groups of n.
void Fault(byte n) {
  cli(); // No more interrupts.
  while (true) {
    for (byte k = 0; k < n; k++) {
  	for (word i = 0; i < 8; i++) {
  		SpinDelay(400);
		LedOn();
		SpinDelay(100);
		LedOff();
	}
	SpinDelay(5000);
    }
    SpinDelay(8000);
  }
}

template <int PB>
struct MoctalTicker {
  
  // You must zero these yourself, if not global.
  volatile byte data;     // Number to ouptut.
  volatile byte shifted;  // High bit is output; shifts to the left.
  volatile byte tick;     // Counts within states.
  volatile byte state;    // Counts bits and gaps.

  static void Setup() {
    pinMode(PB, OUTPUT);
  }  

  void Tick() {
    if (tick == 0) {
      // MOVE
      if (state==0 || state==8) {
        shifted = data;
        state = 1;
      } else {
        shifted <<= 1;
        ++state;
      }
      
      if (shifted & 0x80) {
        tick = 5;  // Long pulse.
      } else {
        tick = 2;  // Short pulse.
      }
      
      if (pgm_read_byte(MoctalTick_GapFollows+state)) {
        tick += 2;  // Pulse followed by longer gap.
      }
    }
    
    if (tick > (pgm_read_byte(MoctalTick_GapFollows+state) ? 3 : 1)) {
      LedOn();
    } else {
      LedOff();
    }
    tick--;
  }
};
MoctalTicker<0> moc;

// Timer/Counter 1 PWM Output OC1A (PB1)
struct FastPwm1Base {
  static void SetupPLL() {
    // de https://github.com/viking/attiny85-player //
    PLLCSR |= _BV(PLLE);               // Enable 64 MHz PLL (p94)
    SpinDelay(1); /// delayMicroseconds(100);            // Stabilize
    while(!(PLLCSR & _BV(PLOCK)));     // Wait for it... (p94)
    PLLCSR |= _BV(PCKE);               // Timer1 source = PLL
  };
};
struct FastPwm1A : public FastPwm1Base {
  static void Setup() {
    SetupPLL();
  
    // Set up Timer/Counter1 for PWM output
    TIMSK  = 0;                        // Timer interrupts OFF (p92)
    TCCR1  = _BV(PWM1A)                // Pulse Width Modulator A Enable. (p89)
           | _BV(COM1A1)               // Clear OC1A on match; set on count $00.
           | _BV(CS10);                // 1:1 prescale (p89)
    GTCCR  = 0;                        // Do not enable PW Modulater B. (p90)
    OCR1C  = 255;                      // Full 8-bit PWM cycle (p92)
    OCR1B  = 0;                        // Not used.
    OCR1A  = 128;                      // 50% duty at start

    pinMode(1, OUTPUT);                // Enable PWM output OC1A on pin PB1.
  }
  static void Output(int x) {
    OCR1A = x;  // (p92)
  }
};

struct AnalogIn {
  static void NextInputB() {
    ADMUX = 0 |    // Use Vcc for Reference; disconnect from PB0 (p134)
            _BV(ADLAR) | // Left-Adjust the ADC Result in ADCH (p134)
            2;     // ADC2 is PB4 is B.
  }
  static void NextInputA() {
    ADMUX = 0 |    // Use Vcc for Reference; disconnect from PB0 (p134)
            _BV(ADLAR) | // Left-Adjust the ADC Result in ADCH (p134)
            3;     // ADC3 is PB3 is A.
  }
  static void NextInputR() {
    ADMUX = 0 |    // Use Vcc for Reference; disconnect from PB0 (p134)
            _BV(ADLAR) | // Left-Adjust the ADC Result in ADCH (p134)
            0;     // ADC0 is PB5 is R.
  }
  static void Setup() {
    ADCSRA = _BV(ADEN);  // Enable ADC first.
    
    DIDR0 = 0x3F;  // shut down digital inputs to save power (p138)

    NextInputA();
            
    ADCSRB = 0; // free-running ADC; not Bipolar; not reversed. (p137)

    // (p125) max ADC res: clock 50kHz to 200kHz
    // If less than 10 bits, 200khz to 1MHz.
    // We use internal 16MHz clock, so try dividing by 32.
    // 1->/2 2->/4 3->/8 4->/16 5->/32 6->/64 7->/128  (p136)
    // 7->4.88kHz   6->9.75kHz  5->19.53kHz Nyquist (freq of LED 2xToggle)  
    // Use 6:   16MHz / 64 => 250kHz.    
    // 250kHz / 13 => 19,230 samples per sec; nyquist 9615 Hz
    // Use 5: Can do two samples, 9615 Hz.
#define DIVISOR 5
    byte tmp = _BV(ADEN) |  // Enable the Analog-to-Digital converter (p136)
              _BV(ADATE) |  // Enable ADC auto trigger
              _BV(ADIE) |  // Enable interrupt on Conversion Complete
              DIVISOR;
    // ADCSRA = tmp;
    ADCSRA = tmp | _BV(ADSC);  // Start the fist conversion
  }
  static byte Input() {
    return ADCH;   // Just 8-bit.  Assumes ADLAR is 1. 
  }
};

volatile byte AnalogA, AnalogB, AnalogR;
AnalogIn in;
FastPwm1A out;
volatile byte adc_counter;
volatile bool adc_switch;

ISR(ADC_vect) {
  if (adc_switch) {
    ++adc_counter;
    adc_switch = false;
    // One time out of 256 we sample R.  The rest we sample B.
    if (adc_counter == 2) {
      AnalogB = ADCH;          // on 2, we still save B, but request R next.
      AnalogIn::NextInputR();
    } else if (adc_counter == 3) {
      AnalogR = ADCH;          // on 3, we save R, but request B again.
      AnalogIn::NextInputB();
      //LedToggle();
    } else {
      AnalogB = ADCH;
      AnalogIn::NextInputB();
    }
  } else {
    adc_switch = true;
    AnalogA = ADCH;
    AnalogIn::NextInputA();
  }
}

void setup() {
  PORTB = 0x3E;  // pull ups
  DDRB = 0x03;   // Only PB0 and PB1 are outputs.
  DDRB = 0x03;   // Only PB0 and PB1 are outputs.
  //pinMode(0, OUTPUT);  // C: LED on SparkFun ATtiny programmer.
  LedOff();
  //pinMode(1, OUTPUT);  // F: PWM out
  //pinMode(2, INPUT_PULLUP);  // D: 
  //pinMode(3, INPUT_PULLUP);  // A: Analog-to-Digital Converter Input PB3 (pin 2)
  //pinMode(4, INPUT_PULLUP);  // B: Analog-to-Digital Converter Input PB4 (pin 3)
  //pinMode(5, INPUT_PULLUP);  // R: RESET pin.

  out.Setup();
  in.Setup();
  moc.Setup();
  moc.data = 0;

  ::Setup();  // Call user's Setup.
}

void loop() {
  // no need to return; just loop here.
  while (true) {


    {
      byte old_counter = adc_counter;
      byte c;
#if MOC_EXTRA_LOOP_COUNT 
      word loops = 0;
#endif
      do {
#if MOC_EXTRA_LOOP_COUNT 
        ++loops;
#endif
        c = adc_counter;
      } while (old_counter == c);
      if (byte(old_counter+1) == c) {
#if MOC_EXTRA_LOOP_COUNT 
        moc.data = loops;
#endif
      } else {
        Fault(3); // Fault on overruns.  May relax this.
      }
      old_counter=c;
    }

    ::Loop();  // Call user's Loop.

#if MOC_TICKS
    if (adc_counter == 0) {
    	    static byte tick_counter;
	    ++tick_counter;
	    if ((tick_counter & 15) == 0) {
		moc.Tick();
	    }
    }
#endif

  }
}

}

void setup() { gerboa_internal::setup(); }
void loop() { gerboa_internal::loop(); }

#define IN_A()   (gerboa_internal::AnalogA)
#define IN_B()   (gerboa_internal::AnalogB)
#define IN_R()   (gerboa_internal::AnalogR)
#define OUT_F(B) (OCR1A = (B))
#define MOCTAL(B)  (gerboa_internal::moc.data = (B))
using gerboa_internal::LedOn;
using gerboa_internal::LedOff;
using gerboa_internal::LedToggle;
using gerboa_internal::Fault;

#endif
