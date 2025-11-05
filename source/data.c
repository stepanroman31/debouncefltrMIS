#include "data.h"

static int16_t potValue_i = 0;
static bool S1_output_i = false;
static bool S2_output_i = false;

// Settery
void setPotValue(int16_t value) {
    if (value > 2047) {
        potValue_i = 2047;
    } else if (value < -2047) {
        potValue_i = -2047;
    } else {
        potValue_i = value;
    }
}

void setS1Output(bool state) {
    S1_output_i = state;
}

void setS2Output(bool state) {
    S2_output_i = state;
}

// Gettery
int16_t getPotValue(void) {
    return potValue_i;
}

bool getS1Output(void) {
    return S1_output_i;
}

bool getS2Output(void) {
    return S2_output_i;
}
