#include "display.h"
#include <stdbool.h>
#include <stdio.h>
// Defines
#define DEBUG false
#define WAITING_STATE "WAITING \n"
#define ADC_SETTLING_STATE "ADC_SETTLING \n"
#define PRESS_ST_ST "PRESS_ST\n"
#define ERR_MSG "ERROR\n"
#define ADC_WAIT_TIME .05
// Global Variables
double tick_timer;
bool pressed = false;
uint32_t adc_timer;
uint32_t adc_settle_ticks;
int16_t x, y;
uint8_t z;

// Status of the touchscreen
typedef enum {
  TOUCHSCREEN_IDLE,    // Touchscreen is idle (not pressed)
  TOUCHSCREEN_PRESSED, // Touchscreen is actively being pressed
  TOUCHSCREEN_RELEASED // Touchscreen has been touched,
} touchscreen_status_t;

// States
typedef enum {
  WAITING,      // Touchscreen is idle (not pressed)
  ADC_SETTLING, // Touchscreen is actively being pressed
  PRESSED       // Touchscreen has been released, but not acknowledged
} touchscreen_state_t;
touchscreen_status_t ts_status;
touchscreen_state_t ts_state;

// Initialize the touchscreen driver state machine, with a given tick period (in
// seconds).
void touchscreen_init(double period_seconds) {
  tick_timer = period_seconds;
  adc_settle_ticks = ADC_WAIT_TIME / period_seconds;
  ts_state = WAITING;
  ts_status = TOUCHSCREEN_IDLE;
};

// This is a debug state print routine. It will print the names of the states
// each time tick() is called. It only prints states if they are different than
// the previous state
void debugStatePrint() {
  static touchscreen_state_t previousState;
  static bool firstPass = true;
  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state
  // name over and over. Return the current status of the touchscreen
  if (previousState != ts_state || firstPass) {
    firstPass = false;
    previousState = ts_state;
    switch (ts_state) {
    case WAITING:
      printf(WAITING_STATE);
      break;
    case ADC_SETTLING:
      printf(ADC_SETTLING_STATE);
      break;
    case PRESSED:
      printf(PRESS_ST_ST);
      break;
    }
  }
}

// Tick the touchscreen driver state machine
void touchscreen_tick() {
  if (DEBUG) {
    debugStatePrint();
  }
  // State Transisition
  switch (ts_state) {
  case WAITING:
    if (display_isTouched()) {
      display_clearOldTouchData();
      ts_state = ADC_SETTLING;
    }
    break;
  case ADC_SETTLING:
    if (display_isTouched() && (adc_timer == adc_settle_ticks)) {
      display_getTouchedPoint(&x, &y, &z);
      ts_state = PRESSED;
      ts_status = TOUCHSCREEN_PRESSED;
    } else if (!display_isTouched()) {
      ts_state = WAITING;
      ts_status = TOUCHSCREEN_IDLE;
    }
    break;
  case PRESSED:
    if (!display_isTouched()) {
      ts_state = WAITING;
      ts_status = TOUCHSCREEN_RELEASED;
    }
    break;
  default:
    printf(ERR_MSG);
  }
  // State Action
  switch (ts_state) {
  case WAITING:
    adc_timer = 0;
    break;
  case ADC_SETTLING:
    adc_timer++;
    break;
  case PRESSED:
    pressed = true;
    break;
  default:
    printf(ERR_MSG);
  }
};
// Return the status of the touchscreen
touchscreen_status_t touchscreen_get_status() { return ts_status; };

// Acknowledge the touchscreen touch.  This function will only have effect when
// the touchscreen is in the TOUCHSCREEN_RELEASED status, and will cause it to
// switch to the TOUCHSCREEN_IDLE status.
void touchscreen_ack_touch() {
  if (ts_status == TOUCHSCREEN_RELEASED) {
    ts_status = TOUCHSCREEN_IDLE;
  }
  pressed = false;
}

// Get the (x,y) location of the last touchscreen touch
display_point_t touchscreen_get_location() {
  display_point_t temp = {x, y};
  return temp;
};
