#include "data.h"

static int16_t potValue_i = 0;
static bool S1_output_i = false;
static bool S2_output_i = false;
static bool S3_output_i = false;

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
void setS3Output(bool state) {
    S3_output_i = state;
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

// Nové statické prom?nné
static uint8_t switched_output_i = 0;
static bool led_v9_i = false;
static bool led_v12_i = false;

// Nové settery
void setSwitchedOutput(uint8_t value) {
    switched_output_i = value;
}
void setLedV9(bool state) {
    led_v9_i = state;
}
void setLedV12(bool state) {
    led_v12_i = state;
}

// Nové gettery
uint8_t getSwitchedOutput(void) {
    return switched_output_i;
}
bool getLedV9(void) {
    return led_v9_i;
}
bool getLedV12(void) {
    return led_v12_i;
}
