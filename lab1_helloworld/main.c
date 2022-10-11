/*
This software is provided for student assignment use in the Department of
Electrical and Computer Engineering, Brigham Young University, Utah, USA.

Users agree to not re-host, or redistribute the software, in source or binary
form, to other persons or other institutions. Users may modify and use the
source code for personal or educational use.

For questions, contact Brad Hutchings or Jeff Goeders, https://ece.byu.edu/
*/

#include <stdio.h>

#include "display.h"

#define TEXT_SIZE 2
#define CURSOR_X 10
#define CURSOR_Y (DISPLAY_HEIGHT / 2)

// Shapes
#define X_CIRCLE_EMPTY 70
#define Y_CIRCLE_EMPTY DISPLAY_HEIGHT / 2
#define R_CIRCLE_EMPTY 30

#define X_CIRCLE_FILL 245
#define Y_CIRCLE_FILL DISPLAY_HEIGHT / 2
#define R_CIRCLE_FILL 30

#define X1_TRIANGLE_EMPTY DISPLAY_WIDTH / 2 + 30
#define Y1_TRIANGLE_EMPTY 226
#define X2_TRIANGLE_EMPTY DISPLAY_WIDTH / 2 - 30
#define Y2_TRIANGLE_EMPTY 226
#define X3_TRIANGLE_EMPTY DISPLAY_WIDTH / 2
#define Y3_TRIANGLE_EMPTY 160

#define X1_TRIANGLE_FILL DISPLAY_WIDTH / 2 - 30
#define Y1_TRIANGLE_FILL 30
#define X2_TRIANGLE_FILL DISPLAY_WIDTH / 2 + 30
#define Y2_TRIANGLE_FILL 30
#define X3_TRIANGLE_FILL DISPLAY_WIDTH / 2
#define Y3_TRIANGLE_FILL 100

#define X_LINE_A_START 0
#define Y_LINE_A_START 0

#define X_LINE_A_END DISPLAY_WIDTH
#define Y_LINE_A_END DISPLAY_HEIGHT

#define X_LINE_B_START 0
#define Y_LINE_B_START DISPLAY_HEIGHT

#define X_LINE_B_END DISPLAY_WIDTH
#define Y_LINE_B_END 0

// Print out "hello world" on both the console and the LCD screen.
int main() {

  // Initialize display driver, and fill scren with black
  display_init();
  display_fillScreen(DISPLAY_BLACK); // Blank the screen.

  // Configure display text settings
  display_setTextColor(DISPLAY_RED); // Make the text red.
  display_setTextSize(TEXT_SIZE);    // Make the text a little larger.

  // Set the cursor location and print to the LCD
  display_setCursor(CURSOR_X, CURSOR_Y);

  // Also print out 'hello world' on the terminal (stdout).
  printf("hello world!\n");

  // Triangles
  display_drawTriangle(X1_TRIANGLE_EMPTY, Y1_TRIANGLE_EMPTY, X2_TRIANGLE_EMPTY,
                       Y2_TRIANGLE_EMPTY, X3_TRIANGLE_EMPTY, Y3_TRIANGLE_EMPTY,
                       DISPLAY_WHITE);
  display_fillTriangle(X1_TRIANGLE_FILL, Y1_TRIANGLE_FILL, X2_TRIANGLE_FILL,
                       Y2_TRIANGLE_FILL, X3_TRIANGLE_FILL, Y3_TRIANGLE_FILL,
                       DISPLAY_WHITE);

  // Cirles
  display_fillCircle(X_CIRCLE_FILL, Y_CIRCLE_FILL, R_CIRCLE_FILL, DISPLAY_RED);
  display_drawCircle(X_CIRCLE_EMPTY, Y_CIRCLE_EMPTY, R_CIRCLE_EMPTY,
                     DISPLAY_RED);

  // Lines
  display_drawLine(X_LINE_A_START, Y_LINE_A_START, X_LINE_A_END, Y_LINE_A_END,
                   DISPLAY_GREEN);
  display_drawLine(X_LINE_B_START, Y_LINE_B_START, X_LINE_B_END, Y_LINE_B_END,
                   DISPLAY_GREEN);
  return 0;
}
