

#ifndef DATA_H
#define	DATA_H

#include "platformMIS32mk.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    PLC_PROG,
    PLC_TEST,
    PLC_RUN,
    PLC_STOP
} plcState_t;
// Deklarace globalnich getteru a setteru pro data
void setS1Output(bool state);
bool getS1Output(void);

void setS3Output(bool state);
bool getS3Output(void);

void setS2Output(bool state);
bool getS2Output(void);

void setRtmParameter(uint8_t value);
uint8_t getRtmParameter(void);

// Nové settery a gettery pro DEK
void setSwitchedOutput(uint8_t value);
uint8_t getSwitchedOutput(void);

void setLedV9(bool state);
bool getLedV9(void);

void setLedV12(bool state);
bool getLedV12(void);

void setRtmCommand(signed short cmd);
signed short getRtmCommand(void);

void setPlcState(plcState_t state);
plcState_t getPlcState(void);

void setPlcCurrentIndex(uint8_t index);
uint8_t getPlcCurrentIndex(void);

void setPlcCurrentValue(uint8_t value); // Hodnota 0-90
uint8_t getPlcCurrentValue(void);

void setRtmCommand(signed short cmd);
signed short getRtmCommand(void);

// PWM p?epo?et a sekvence
void setPlcSequenceValue(uint8_t index, uint8_t value);
uint8_t getPlcSequenceValue(uint8_t index);
void setPlcSequenceLength(uint8_t len);
uint8_t getPlcSequenceLength(void);

#endif	/* DATA_H */

