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
    for (byte i = 0; i < 8; i++) { ClockUp(); SpinDelayFast(20); ClockDown(); SpinDelayFast(40); }
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
 public:
  static void Setup() {
    pinMode(0, INPUT);  // Data In
    pinMode(2, INPUT);  // Clock In
    ControlReg = kThreeWireMode | kExternalClockPositiveEdge;
  }
  static byte Receive() {
    DataReg = 0xFE;  // would be sent back to master, if we cared.
    StatusReg = _BV(USIOIF);  // Clear the counter Overflow Interrupt Flag.
    // Wait for counter Overflow:
    while ((StatusReg & _BV(USIOIF)) == 0) {
    	LedToggle();
    	Fault(2);
    }
    return DataReg;
  }
};
#undef DataReg
#undef BufferReg
#undef StatusReg
#undef ControlReg

#endif
