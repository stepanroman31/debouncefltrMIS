

#ifndef DATA_H
#define	DATA_H

#include "platformMIS32mk.h"
#include <stdbool.h>
#include <stdint.h>

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

#endif	/* DATA_H */

