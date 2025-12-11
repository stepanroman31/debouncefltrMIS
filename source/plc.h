/* 
 * File:   plc.h
 * Author: ?t?pán
 *
 * Created on 11. prosince 2025, 10:33
 */

#ifndef PLC_H
#define	PLC_H

#include <stdint.h>
#include <stdbool.h>

void initPLC(void);
void runPLC(bool btnStop, bool btnRun, bool btnReset, bool btnSet, bool btnTest, uint8_t encoderVal);

// Funkce pro p?epo?et 0-255 -> 0-90 [cite: 343]
uint8_t scaleTo90(uint8_t input255);
// Funkce pro p?epo?et 0-90 -> 0-255 (pro PWM)
uint8_t scaleTo255(uint8_t input90);

#endif

