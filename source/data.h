

#ifndef DATA_H
#define	DATA_H

#include "platformMIS32mk.h"
#include <stdbool.h>
#include <stdint.h>

// Deklarace globalnich getteru a setteru pro data
void setPotValue(int16_t value);
int16_t getPotValue(void);

void setS1Output(bool state);
bool getS1Output(void);

void setS2Output(bool state);
bool getS2Output(void);

#endif	/* DATA_H */

