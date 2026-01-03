/* 
 * File:   pwmmod.h
 * Author: ?t?pán
 *
 * Created on 19. listopadu 2025, 16:54
 */

#ifndef PWMMOD_H
#define	PWMMOD_H

#include <stdint.h>
#include <xc.h>
#include <stdbool.h>

void initPwm(void);
void updatePwm(uint8_t input_val);
uint8_t getPwmLastValue(void);

#endif	/* PWMMOD_H */

