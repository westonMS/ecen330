#include "gpioTest.h"
#include "buttons.h"
#include "display.h"
#include "leds.h"
#include "switches.h"
#include <stdbool.h>
#include <stdio.h>
#define ALL_ON 0xF
#define OFF 0x0
#define ORIGIN_X 0
#define ORIGIN_Y 0
#define WIDTH DISPLAY_WIDTH / 4
#define HEIGHT DISPLAY_HEIGHT / 2
#define X_BUTTON0 DISPLAY_WIDTH * 3 / 4
#define X_BUTTON1 DISPLAY_WIDTH * 2 / 4
#define X_BUTTON2 DISPLAY_WIDTH * 1 / 4
#define X_BUTTON3 DISPLAY_WIDTH * 0 / 4
#define TEXT_SIZE 2
#define TEXT_OFFSET 15
#define CURSOR_Y DISPLAY_HEIGHT / 4
void gpioTest_buttons() {
  display_init();
  display_setTextSize(TEXT_SIZE);
  display_setTextColor(DISPLAY_BLACK);
  display_fillScreen(DISPLAY_BLACK); // Make the text a little larger.
  uint8_t buttons = OFF;
  buttons_init();
  bool button_0_drawn, button_1_drawn, button_2_drawn, button_3_drawn;
  button_0_drawn = false;
  button_1_drawn = false;
  button_2_drawn = false;
  button_3_drawn = false;
  while (buttons != ALL_ON) {
    buttons = buttons_read();
    if ((button_0_drawn == false) &&
        ((buttons & BUTTONS_BTN0_MASK) == BUTTONS_BTN0_MASK)) {
      // Create the triangle
      display_fillRect(X_BUTTON0, ORIGIN_Y, WIDTH, HEIGHT, DISPLAY_GREEN);
      printf("Button 0 Pressed \n");
      display_setCursor(X_BUTTON0 + TEXT_OFFSET, CURSOR_Y);
      display_println("BTN0");
      button_0_drawn = true;

    } else if ((button_0_drawn == true) &&
               ((buttons & BUTTONS_BTN0_MASK) == OFF)) {
      // Turn off the triangle
      display_fillRect(X_BUTTON0, ORIGIN_Y, WIDTH, HEIGHT, DISPLAY_BLACK);

      printf("Button 0 Released\n");
      button_0_drawn = false;
    }
    if ((button_1_drawn == false) &&
        ((buttons & BUTTONS_BTN1_MASK) == BUTTONS_BTN1_MASK)) {
      // Create the triangle
      display_fillRect(X_BUTTON1, ORIGIN_Y, WIDTH, HEIGHT, DISPLAY_BLUE);

      printf("Button 1 Pressed\n");
      display_setCursor(X_BUTTON1 + TEXT_OFFSET, CURSOR_Y);
      display_println("BTN1");
      button_1_drawn = true;
    } else if ((button_1_drawn == true) &&
               ((buttons & BUTTONS_BTN1_MASK) == OFF)) {
      // Turn off the triangle
      display_fillRect(X_BUTTON1, ORIGIN_Y, WIDTH, HEIGHT, DISPLAY_BLACK);

      printf("Button 1 Released\n");
      button_1_drawn = false;
    }
    if (button_2_drawn == false &&
        ((buttons & BUTTONS_BTN2_MASK) == BUTTONS_BTN2_MASK)) {
      // Create the triangle
      display_fillRect(X_BUTTON2, ORIGIN_Y, WIDTH, HEIGHT, DISPLAY_YELLOW);

      printf("Button 2 Pressed\n");
      display_setCursor(X_BUTTON2 + TEXT_OFFSET, CURSOR_Y);
      display_println("BTN2");
      button_2_drawn = true;
    } else if (button_2_drawn == true &&
               ((buttons & BUTTONS_BTN2_MASK) == OFF)) {
      // Turn off the triangle
      display_fillRect(X_BUTTON2, ORIGIN_Y, WIDTH, HEIGHT, DISPLAY_BLACK);

      printf("Button 2 Released\n");
      button_2_drawn = false;
    }
    if ((button_3_drawn == false) &&
        ((buttons & BUTTONS_BTN3_MASK) == BUTTONS_BTN3_MASK)) {
      // Create the triangle
      display_fillRect(X_BUTTON3, ORIGIN_Y, WIDTH, HEIGHT, DISPLAY_WHITE);

      printf("Button 3 Pressed\n");
      display_setCursor(X_BUTTON3 + TEXT_OFFSET, CURSOR_Y);
      display_println("BTN3");
      button_3_drawn = true;
    } else if ((button_3_drawn == true) &&
               ((buttons & BUTTONS_BTN3_MASK) == OFF)) {
      // Turn off the triangle
      display_fillRect(X_BUTTON3, ORIGIN_Y, WIDTH, HEIGHT, DISPLAY_BLACK);

      printf("Button 3 Released\n");
      button_3_drawn = false;
    }
  }
  display_fillScreen(DISPLAY_BLACK);
};
void gpioTest_switches() {
  leds_init();
  switches_init();
  uint8_t switches = OFF;
  while (switches != ALL_ON) {
    switches = switches_read();
    leds_write(switches);
  }
  leds_write(OFF);
}
