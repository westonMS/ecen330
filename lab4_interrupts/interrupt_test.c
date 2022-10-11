#include "interrupts.h"
#include "intervalTimer.h"
#include "leds.h"

#define TEN_SEC 10
#define ONE_SEC 1
#define TENTH_SEC .1
#define LED_ZERO 0x1
#define LED_ONE 0x2
#define LED_TWO 0x4
#define TIMER0 0
#define TIMER1 1
#define TIMER2 2

/*
This function is a small test application of your interrupt controller.  The
goal is to use the three AXI Interval Timers to generate interrupts at different
rates (10Hz, 1Hz, 0.1Hz), and create interrupt handler functions that change the
LEDs at this rate.  For example, the 1Hz interrupt will flip an LED value each
second, resulting in LED that turns on for 1 second, off for 1 second,
repeatedly.

For each interval timer:
    1. Initialize it as a count down timer with appropriate period (1s, 0.5s,
0.25s)
    2. Enable the timer's interrupt output
    3. Enable the associated interrupt input on the interrupt controller.
    4. Register an appropriate interrupt handler function (isr_timer0,
isr_timer1, isr_timer2)
    5. Start the timer

Make sure you call `interrupts_init()` first!
*/
// Interrupt Service Routine

// Timer 0
void timer0_isr() {
  int leds = leds_read();
  leds = leds ^ LED_ZERO;
  leds_write(leds);
}
// Timer 1
void timer1_isr() {
  int leds = leds_read();
  leds = leds ^ LED_ONE;
  leds_write(leds);
}
// Timer 2
void timer2_isr() {
  int leds = leds_read();
  leds = leds ^ LED_TWO;
  leds_write(leds);
}
// Test and Inits
void interrupt_test_run() {
  interrupts_init();
  leds_init();
  // Register Time Functions
  interrupts_register(TIMER0, timer0_isr);
  interrupts_register(TIMER1, timer1_isr);
  interrupts_register(TIMER2, timer2_isr);
  // Register time 0 - 2
  intervalTimer_initCountDown(TIMER0, TENTH_SEC);
  intervalTimer_initCountDown(TIMER1, ONE_SEC);
  intervalTimer_initCountDown(TIMER2, TEN_SEC);
  intervalTimer_enableInterrupt(TIMER0);
  intervalTimer_enableInterrupt(TIMER1);
  intervalTimer_enableInterrupt(TIMER2);

  // Interrupt ENABLE
  interrupts_irq_enable(TIMER0);
  interrupts_irq_enable(TIMER1);
  interrupts_irq_enable(TIMER2);

  // Start Timers
  intervalTimer_start(TIMER0);
  intervalTimer_start(TIMER1);
  intervalTimer_start(TIMER2);
  while (true) {
  }
};