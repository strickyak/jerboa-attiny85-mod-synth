#ifndef NET1_H_
#define NET1_H_

// net1.h -- network of 1-way (single duplex) master-to-slave commands.
//
// attiny{25,45,85} Serial registers:
#define DataREG USIDR
#define BufferREG USIBR
#define StatusREG USISR
#define ControlREG USICR

class Master {
  // Clock out: pin7 = PB2 = USCK = Clock Out
  // Data out:  pin6 = PB1 = DO = Data Out
  // Data in ignored: pin 5 = PB0

  static constexpr byte kThreeWireMode = _BV(USIWM0);
  static constexpr byte kNoClock = 0;  // clock is in software.
  static constexpr byte kToggleClock = _BV(USITC);
  static constexpr byte kStrobe = _BV(USICLK);

  static void ClockUp() { ControlREG = kThreeWireMode | kNoClock | kToggleClock | 0; }
  static void ClockDown() { ControlREG = kThreeWireMode | kNoClock | kToggleClock | kStrobe; }

 public:
  static void Send(byte data) {
    DataREG = data;
    for (byte i = 0; i < 8; i++) { ClockUp(); SpinDelayFast(15); ClockDown(); SpinDelayFast(15); }
    ControlREG = 0;
  }
  static void Setup() {
    pinMode(1, OUTPUT);  // Data OUT
    pinMode(2, OUTPUT);  // Clock Out
  }
};

class Slave {
  static constexpr byte kThreeWireMode = _BV(USIWM0);
  static constexpr byte kExternalClockPositiveEdge = _BV(USICS1);
  static constexpr byte kExternalClockNegativeEdge = _BV(USICS1) | _BV(USICS0);
  static constexpr byte kCounterOverflowInterruptEnable = _BV(USIOIE);
 public:
  static volatile byte slave_data;
  static volatile byte slave_ready;
  static volatile byte slave_toggle;

  static void Setup() {
    pinMode(0, INPUT);  // Data In
#if 1
    pinMode(1, INPUT);     // Try another input.
    PCMSK |= 1<<1;         // Pin-Change interrupt on PB1.
    //////// MCUCR |= 3;            // Interrupt on rising edge.
    GIMSK |= _BV(PCIE);    // Pin change interrupt enable
#else
    pinMode(1, OUTPUT);  // Data Out
#endif
    pinMode(2, INPUT);  // Clock In
    StatusREG = _BV(USIOIF);  // Clear the counter Overflow Interrupt Flag.
    ControlREG = kThreeWireMode | kExternalClockPositiveEdge | kCounterOverflowInterruptEnable;
  }
  static byte Receive() {
    slave_ready = false;
    DataREG = 0xFE;  // would be sent back to master, if we cared.
/**/
    while (true) {
      byte sr = slave_ready;
      if (sr) break;
    }
    slave_ready = false;
    return slave_data;

/*
    StatusREG = _BV(USIOIF);  // Clear the counter Overflow Interrupt Flag.
    // Wait for counter Overflow:
    while ((StatusREG & _BV(USIOIF)) == 0) {
    	LedToggle();
    }
    return DataREG;
*/
  }
};

volatile byte Slave::slave_data;
volatile byte Slave::slave_ready;
volatile byte Slave::slave_toggle;

ISR(USI_OVF_vect) {
  StatusREG = _BV(USIOIF);  // Clear the counter Overflow Interrupt Flag.
  Slave::slave_data = DataREG;     // Read the byte from the shift register.

#if 0
  if (Slave::slave_data == 0) {
    // Synchronize.
    while (DataREG != 0xFF) {
      StatusREG = _BV(USIOIF) | 0x0E;  // Clear the counter Overflow Interrupt Flag, and set counter for 1 bit (2 edges).
      while (!(StatusREG & _BV(USIOIF))) {
        continue; // Wait until overflow flag, meaning we got 1 more bit.
      }
    }
    Slave::slave_data == 0;  // Return 0 after sync.
  }
#endif

  Slave::slave_toggle = !Slave::slave_toggle;
  Slave::slave_ready = true;
}

//XX volatile bool led_blink;
ISR(PCINT0_vect) {
  if (PINB & (1<<1)) {
    StatusREG = _BV(USIOIF);  // Clear the counter Overflow Interrupt Flag.
    //XX led_blink = !led_blink;
    //XX digitalWrite(WHICH_LED, led_blink ? HIGH : LOW);
  }
}

#undef DataREG
#undef BufferREG
#undef StatusREG
#undef ControlREG

#endif
