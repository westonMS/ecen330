#include "armInterrupts.h"
#include "intervalTimer.h"
#include "xil_io.h"
#include <leds.h>
#include <stdint.h>
#include <xparameters.h>

#define IACKR_OFFSET 0x0C
#define MER_OFFSET 0x1C
#define MER_VALUE 0x3
#define SIE_OFFSET 0x10
#define CIE_OFFSET 0x14
#define CIE_DISABLE_ALL 0xFF
#define IPR_OFFSET 0x04
#define NUMBER_OF_ISR 3

// GLOBAL
static void (*isrFcnPtrs[NUMBER_OF_ISR])() = {NULL};

// Read a Given Register
static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_AXI_INTC_0_BASEADDR +
                  offset); // Writes value to the address.
};

// Write to a given Register
static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + offset, value);
};
// Interrupt ISR
static void interrupts_isr() {
  // Loop through each interrupt input
  for (int i = 0; i < NUMBER_OF_ISR; i++) {
    // Check if it has an interrupt pending
    if (readRegister(IPR_OFFSET) & (1 << i)) {
      // Check if there is a callback
      if (isrFcnPtrs[i])
        // Call the callback function
        isrFcnPtrs[i]();
      // ACK
      intervalTimer_ackInterrupt(i);
      writeRegister(IACKR_OFFSET, (1 << i));
    }
  }
}

// Initialize interrupt hardware
// This function should:
// 1. Configure AXI INTC registers to:
//  - Enable interrupt output (see Master Enable Register)
//  - Disable all interrupt input lines.
// 2. Enable the Interrupt system on the ARM processor, and register an ISR
// handler function. This is done by calling:
//  - armInterrupts_init()
//  - armInterrupts_setupIntc(isr_fcn_ptr)
//  - armInterrupts_enable()
void interrupts_init() {
  // Configure AXI
  writeRegister(MER_OFFSET, MER_VALUE);
  // Disable All Interrupt Input Lines
  writeRegister(CIE_OFFSET, CIE_DISABLE_ALL);
  // Enable Handlers
  armInterrupts_init();
  armInterrupts_setupIntc(interrupts_isr);
  armInterrupts_enable();
};

// Register a callback function (fcn is a function pointer to this callback
// function) for a given interrupt input number (irq).  When this interrupt
// input is active, fcn will be called.
void interrupts_register(uint8_t irq, void (*fcn)()) { isrFcnPtrs[irq] = fcn; };

// Enable single input interrupt line, given by irq number.
void interrupts_irq_enable(uint8_t irq) {
  uint8_t position;
  position = 1 << irq;
  writeRegister(SIE_OFFSET, position); // SIE register
};

// Disable single input interrupt line, given by irq number.
void interrupts_irq_disable(uint8_t irq) {
  uint8_t position;
  position = 1 << irq;
  writeRegister(CIE_OFFSET, position); // CIE register
};
