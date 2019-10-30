#ifndef NET1_H_
#define NET1_H_

// net1.h -- network of 1-way (single duplex) master-to-slave commands.
//
// attiny{25,45,85} Serial registers:
#define DataReg USIDR
#define BufferReg USIBR
#define StatusReg USISR
#define ControlReg USICR

class Master {
  // Clock out: pin7 = PB2 = USCK = Clock Out
  // Data out:  pin6 = PB1 = DO = Data Out
  // Data in ignored: pin 5 = PB0

  static constexpr byte kThreeWireMode = _BV(USIWM0);
  static constexpr byte kNoClock = 0;  // clock is in software.
  static constexpr byte kToggleClock = _BV(USITC);
  static constexpr byte kStrobe = _BV(USICLK);

  static void ClockUp() { ControlReg = kThreeWireMode | kNoClock | kToggleClock | 0; }
  static void ClockDown() { ControlReg = kThreeWireMode | kNoClock | kToggleClock | kStrobe; }

 public:
  static void Send(byte data) {
    DataReg = data;
    for (byte i = 0; i < 8; i++) { ClockUp(); SpinDelayFast(15); ClockDown(); SpinDelayFast(15); }
    ControlReg = 0;
  }
  static void Setup() {
    pinMode(1, OUTPUT);  // Data OUT
    pinMode(2, OUTPUT);  // Clock Out
  }
};

class Slave {
  static constexpr byte kThreeWireMode = _BV(USIWM0);
  static constexpr byte kExternalClockPositiveEdge = _BV(USICS1);
  static constexpr byte kCounterOverflowInterruptEnable = _BV(USIOIE);
 public:
  static volatile byte slave_data;
  static volatile byte slave_ready;
  static volatile byte slave_toggle;

  static void Setup() {
    pinMode(0, INPUT);  // Data In
    pinMode(1, OUTPUT);  // Data Out
    pinMode(2, INPUT);  // Clock In
    ControlReg = kThreeWireMode | kExternalClockPositiveEdge | kCounterOverflowInterruptEnable;
  }
  static byte Receive() {
    slave_ready = false;
    DataReg = 0xFE;  // would be sent back to master, if we cared.
#if 1
    while (true) {
      byte sr = slave_ready;
      if (sr) break;
    }
    slave_ready = false;
    return slave_data;
#else
    StatusReg = _BV(USIOIF);  // Clear the counter Overflow Interrupt Flag.
    // Wait for counter Overflow:
    while ((StatusReg & _BV(USIOIF)) == 0) {
    	LedToggle();
    }
    return DataReg;
#endif
  }
};
volatile byte Slave::slave_data;
volatile byte Slave::slave_ready;
volatile byte Slave::slave_toggle;

ISR(USI_OVF_vect) {
  StatusReg = _BV(USIOIF);  // Clear the counter Overflow Interrupt Flag.
  Slave::slave_data = DataReg;     // Read the byte from the shift register.
  Slave::slave_ready = true;
  Slave::slave_toggle = !Slave::slave_toggle;
}

#undef DataReg
#undef BufferReg
#undef StatusReg
#undef ControlReg

#endif
