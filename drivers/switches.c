/*
This software is provided for student assignment use in the Department of
Electrical and Computer Engineering, Brigham Young University, Utah, USA.

Users agree to not re-host, or redistribute the software, in source or binary
form, to other persons or other institutions. Users may modify and use the
source code for personal or educational use.

For questions, contact Brad Hutchings or Jeff Goeders, https://ece.byu.edu/
*/
#include "switches.h"
#include "xil_io.h"
#include "xparameters.h"
#define REGISTER_OFFSET 0x0
#define GPIO_TRI_OFF 0xF
#define GPIO_TRI_OFFSET 0x04
#define SWITCHES_MASK 0xF

static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_SLIDE_SWITCHES_BASEADDR +
                  offset); // Writes value to the address.
};

static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_SLIDE_SWITCHES_BASEADDR + offset, value);
};
// Initializes the button driver software and hardware.
void switches_init() { writeRegister(GPIO_TRI_OFFSET, GPIO_TRI_OFF); };

// Returns the current value of all 4 buttons as the lower 4 bits of the
// returned value. bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.
uint8_t switches_read() {
  uint32_t switches;
  switches = readRegister(REGISTER_OFFSET);
  // printf(switches);
  return switches;
};
