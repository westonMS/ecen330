#include "intervalTimer.h"
#include "xil_io.h"
#include "xparameters.h"
#include <stdio.h>
#define TIMER1 0
#define TIMER2 1
#define TIMER3 2
#define BACKWARDS_POS 0x02  // UDT 1
#define CASCADE_POS 0x800   // CASC 11
#define TIMERON_POS 0x80    // ENT 7
#define RELOAD_POS 0x10     // ARTH 4
#define LOAD_POS 0x20       // LOAD 5
#define ALARM_POS 0x100     //  TOINT 8
#define INTERRUPT_POS 0x040 // ENIT 6
#define ON 0x1
#define OFF 0x0
#define CLOCK_RATE XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ
#define TCSR0_OFFSET 0x00
#define TCSR1_OFFSET 0x10
#define TLR0_OFFSET 0x04
#define TCR0_OFFSET 0x08
#define TCR1_OFFSET 0x018
#define TLR1_OFFSET 0x14
#define REGISTER_SHIFT 32

static uint32_t readRegister(uint8_t timerNumber, uint32_t offset) {
  if (timerNumber == TIMER1) {
    return Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + offset);
  } else if (timerNumber == TIMER2) {
    return Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + offset);
  } else if (timerNumber == TIMER3) {
    return Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + offset);
  } else {
    perror("INVALID TIMER");
  }
};
static void writeRegister(uint8_t timerNumber, uint32_t offset,
                          uint32_t value) {
  if (timerNumber == TIMER1) {
    Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + offset, value);
  } else if (timerNumber == TIMER2) {
    Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + offset, value);
  } else if (timerNumber == TIMER3) {
    Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + offset, value);
  } else {
    perror("INVALID TIMER");
  }
};

// You must configure the interval timer before you use it:
// 1. Set the Timer Control/Status Registers such that:
//  - The timer is in 64-bit cascade mode
//  - The timer counts up
// 2. Initialize both LOAD registers with zeros
// 3. Call the _reload function to move the LOAD values into the Counters
void intervalTimer_initCountUp(uint32_t timerNumber) {
  // Cascade and Timer On Register 0
  uint32_t data;
  data = readRegister(timerNumber, TCSR0_OFFSET);
  data |= CASCADE_POS;
  // data |= BACKWARDS_POS;
  writeRegister(timerNumber, TCSR0_OFFSET, data);
  // Cascade and Timer On Register 1
  // Turn on Timer
  // Initialize Load to Zero
  writeRegister(timerNumber, TLR0_OFFSET, OFF);
  writeRegister(timerNumber, TLR1_OFFSET, OFF);
  // Load with Zeros
  intervalTimer_reload(timerNumber);
};

// You must configure the interval timer before you use it:
// 1. Set the Timer Control/Status Registers such that:
//  - The timer is in 64-bit cascade mode
//  - The timer counts down
//  - The timer automatically reloads when reaching zero
// 2. Initialize LOAD registers with appropriate values, given the `period`.
// 3. Call the _reload function to move the LOAD values into the Counters
void intervalTimer_initCountDown(uint32_t timerNumber, double period) {
  uint32_t data;
  data = readRegister(timerNumber, TCSR0_OFFSET);
  data |= CASCADE_POS;
  data |= BACKWARDS_POS;
  data |= RELOAD_POS;
  writeRegister(timerNumber, TCSR0_OFFSET, data);
  // //Cascade and Timer On Register 1
  uint64_t ticks = period * CLOCK_RATE;
  uint32_t upper = ticks >> REGISTER_SHIFT;
  uint32_t lower = ticks;
  writeRegister(timerNumber, TLR0_OFFSET, lower);
  writeRegister(timerNumber, TLR1_OFFSET, upper);
  intervalTimer_reload(timerNumber);
};

// This function starts the interval timer running.
// If the interval timer is already running, this function does nothing.
// timerNumber indicates which timer should start running.
// Make sure to only change the Enable Timer bit of the register and not modify
// the other bits.
void intervalTimer_start(uint32_t timerNumber) {
  uint32_t data;
  data = readRegister(timerNumber, TCSR0_OFFSET);
  data |= TIMERON_POS;
  writeRegister(timerNumber, TCSR0_OFFSET, data);
};

// This function stops a running interval timer.
// If the interval time is currently stopped, this function does nothing.
// timerNumber indicates which timer should stop running.
// Make sure to only change the Enable Timer bit of the register and not modify
// the other bits.
void intervalTimer_stop(uint32_t timerNumber) {
  uint32_t data;
  data = readRegister(timerNumber, TCSR0_OFFSET);
  data = data & ~(TIMERON_POS);
  writeRegister(timerNumber, TCSR0_OFFSET, data);
};

// This function is called whenever you want to reload the Counter values
// from the load registers.  For a count-up timer, this will reset the
// timer to zero.  For a count-down timer, this will reset the timer to
// its initial count-down value.  The load registers should have already
// been set in the appropriate `init` function, so there is no need to set
// them here.  You just need to enable the load (and remember to disable it
// immediately after otherwise you will be loading indefinitely).
void intervalTimer_reload(uint32_t timerNumber) {
  uint32_t data = readRegister(timerNumber, TCSR0_OFFSET);
  data |= LOAD_POS;
  writeRegister(timerNumber, TCSR0_OFFSET, data);
  writeRegister(timerNumber, TCSR1_OFFSET, data);
  data = data & ~(LOAD_POS);
  writeRegister(timerNumber, TCSR0_OFFSET, data);
  writeRegister(timerNumber, TCSR1_OFFSET, data);
};

// Use this function to ascertain how long a given timer has been running.
// Note that it should not be an error to call this function on a running timer
// though it usually makes more sense to call this after intervalTimer_stop()
// has been called. The timerNumber argument determines which timer is read.
// In cascade mode you will need to read the upper and lower 32-bit registers,
// concatenate them into a 64-bit counter value, and then perform the conversion
// to a double seconds value.

double intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber) {
  double seconds;
  uint64_t data;
  uint64_t upper, lower;
  upper = readRegister(timerNumber, TCR1_OFFSET);
  lower = readRegister(timerNumber, TCR0_OFFSET);
  data = upper;
  data = data << REGISTER_SHIFT;
  data |= lower;
  seconds = (double)data / CLOCK_RATE;
  return seconds;
};

// Enable the interrupt output of the given timer.
void intervalTimer_enableInterrupt(uint8_t timerNumber) {
  // Write to ENIT 1
  uint32_t data;
  data = readRegister(timerNumber, TCSR0_OFFSET);
  data |= INTERRUPT_POS;
  writeRegister(timerNumber, TCSR0_OFFSET, data);
};

// Disable the interrupt output of the given timer.
void intervalTimer_disableInterrupt(uint8_t timerNumber) {
  // Write to ENIT 0
  uint32_t data;
  data = readRegister(timerNumber, TCSR0_OFFSET);
  data = data & ~(INTERRUPT_POS);
  writeRegister(timerNumber, TCSR0_OFFSET, data);
};

// Acknowledge the rollover to clear the interrupt output.
void intervalTimer_ackInterrupt(uint8_t timerNumber) {
  // Write to TOINT 1
  uint32_t data;
  data = readRegister(timerNumber, TCSR0_OFFSET);
  data |= ALARM_POS;
  writeRegister(timerNumber, TCSR0_OFFSET, data);
};