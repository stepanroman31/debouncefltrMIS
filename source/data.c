#include "data.h"

static int16_t potValue_i = 0;
static bool S1_output_i = false;
static bool S2_output_i = false;
static bool S3_output_i = false;
static uint8_t RtmParameter = 0;
static signed short RtmCommand = 0;
static plcState_t plcState_i = PLC_PROG; // Výchozí stav [cite: 358]
static uint8_t plcCurrentIndex_i = 0;
static uint8_t plcCurrentValue_i = 0;

#define MAX_SEQ_LEN 50
static uint8_t plcSequence_i[MAX_SEQ_LEN];
static uint8_t plcSequenceLength_i = 0;

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
bool getS3Output(void) {
    return S3_output_i;
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
void setRtmParameter(uint8_t value) {
    // Vstup u? je o?et?en na 0-255 v rtm_com.c
    RtmParameter = value;
}
uint8_t getRtmParameter(void) {
    return RtmParameter;
}
void setRtmCommand(signed short cmd) {
    RtmCommand = cmd;
}
signed short getRtmCommand(void) {
    return RtmCommand;
}

// --- Implementace pro PLC ---
void setPlcState(plcState_t state) { plcState_i = state; }
plcState_t getPlcState(void) { return plcState_i; }

void setPlcCurrentIndex(uint8_t index) { plcCurrentIndex_i = index; }
uint8_t getPlcCurrentIndex(void) { return plcCurrentIndex_i; }

void setPlcCurrentValue(uint8_t value) { plcCurrentValue_i = value; }
uint8_t getPlcCurrentValue(void) { return plcCurrentValue_i; }

void setPlcSequenceValue(uint8_t index, uint8_t value) {
    if (index < MAX_SEQ_LEN) plcSequence_i[index] = value;
}
uint8_t getPlcSequenceValue(uint8_t index) {
    if (index < MAX_SEQ_LEN) return plcSequence_i[index];
    return 0;
}

void setPlcSequenceLength(uint8_t len) {
    if (len <= MAX_SEQ_LEN) plcSequenceLength_i = len;
}
uint8_t getPlcSequenceLength(void) { return plcSequenceLength_i; }
