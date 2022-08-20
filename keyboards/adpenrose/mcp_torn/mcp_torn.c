#include "mcp_torn.h"
#include "i2c_master.h"
#include "mcp23018.h"

void matrix_init_kb(void) {
    // put your keyboard start-up code here
    // runs once when the firmware starts up
    i2c_init();

    matrix_init_user();
}

void matrix_scan_kb(void) {
    // put your looping keyboard code here
    // runs every cycle (a lot)
    if (mcp23018_reset_required()) {
        msp23018_init();
    }
    matrix_scan_user();
}

