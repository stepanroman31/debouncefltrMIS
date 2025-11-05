

#ifndef KOMUNIKACE_H
#define	KOMUNIKACE_H

#include "platformMIS32mk.h"
#include <stdbool.h>
#include <stdint.h>

extern signed short rtmCommand; 

void configRTM(void);
void runRTMCommunication(void);

#endif	/* KOMUNIKACE_H */

