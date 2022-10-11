/*
This software is provided for student assignment use in the Department of
Electrical and Computer Engineering, Brigham Young University, Utah, USA.

Users agree to not re-host, or redistribute the software, in source or binary
form, to other persons or other institutions. Users may modify and use the
source code for personal or educational use.

For questions, contact Brad Hutchings or Jeff Goeders, https://ece.byu.edu/
*/
#include "touchscreen.h"
#include "clockControl.h"
#include "clockDisplay.h"
#include <stdio.h>

#define ERROR "ERROR INVALID STATE\n\r"

#define WAITING_STATE "WAITING\n\r"
#define INC_STATE "INC\n\r"
#define PRESS_STATE "LONG PRESS\n\r"
#define FAST_STATE "FAST UPDATE\n\r"

#define DEL_NUM_TICKS (.5/50E-3)
#define UPD_NUM_TICKS (.5/.1)

#define DEBUG 1
typedef enum {
    WAITING, INC_DEC, LONG_PRESS_DELAY, FAST_UPDATE
} clockControl_state;

clockControl_state current_state;

uint32_t delay_cnt, update_num_ticks, delay_num_ticks, update_cnt;

// Initialize the clock control state machine, with a given period in seconds.
void clockControl_init(double period_s){
    current_state = WAITING;
    update_num_ticks = UPD_NUM_TICKS;
    delay_num_ticks = DEL_NUM_TICKS;
};

//Debug FUnction for Clock state machine
void debug_Clock_Print() {
    static clockControl_state previousState;
    static bool firstPass = true;
    // Only print the message if:
    // 1. This the first pass and the value for previousState is unknown.
    // 2. previousState != currentState - this prevents reprinting the same state
    // name over and over. Return the current status of the touchscreen
    if (previousState != current_state || firstPass) {
        firstPass = false;
        previousState = current_state;
        switch (current_state) {
        case WAITING:
            printf(WAITING_STATE);
            break;
        case INC_DEC:
            printf(INC_STATE);
            break;
        case LONG_PRESS_DELAY:
            printf(PRESS_STATE);
            break;
        case FAST_UPDATE:
            printf(FAST_STATE);
        break;
        }
    }
}

// Tick the clock control state machine
void clockControl_tick(){
    if(DEBUG){
        debug_Clock_Print();
    }

    //Transition
    switch(current_state){
        case WAITING:
            if(touchscreen_get_status() == TOUCHSCREEN_RELEASED){
                current_state = INC_DEC;
            }
            else if(touchscreen_get_status() == TOUCHSCREEN_PRESSED){
                current_state = LONG_PRESS_DELAY;
                delay_cnt = 0;
            }
            break;
        case INC_DEC:
            if(touchscreen_get_status() == TOUCHSCREEN_RELEASED){
                current_state = WAITING;
                touchscreen_ack_touch();
            }
            break;
        case LONG_PRESS_DELAY:
            if(touchscreen_get_status() == TOUCHSCREEN_RELEASED){
                current_state = INC_DEC;
            }
            else if(delay_cnt == delay_num_ticks){
                current_state = FAST_UPDATE;
                update_cnt = 0;
            }
            break;
        case FAST_UPDATE:
            if(!TOUCHSCREEN_RELEASED && (update_cnt == update_num_ticks) ){
                update_cnt = 0;
                clockDisplay_performIncDec(touchscreen_get_location());
            }
            else if(touchscreen_get_status() == TOUCHSCREEN_RELEASED){
                current_state = WAITING;
                touchscreen_ack_touch();
            }
            break;
        default:
            printf(ERROR);
    }
    //Action
    switch(current_state){
        case WAITING:
            break;
        case INC_DEC:
            clockDisplay_performIncDec(touchscreen_get_location());
            break;
        case LONG_PRESS_DELAY:
            delay_cnt++;
            break;
        case FAST_UPDATE:
            update_cnt++;
            break;
    }
};
