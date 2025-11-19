#include "pwmmod.h"
#include <xc.h>
#include <sys/attribs.h>

#define PWM_PERIOD_TICKS 18749 // 20 ms
#define PWM_MIN_TICKS 938       // 1 ms
#define PWM_MAX_TICKS 1875      // 2 ms

void initPwm(void) {
    // 1. Nastavení TIMER 2 (Bod 10d)
    T2CON = 0;              // Vypnout Timer 2
    T2CONbits.TCKPS = 0b110;// Prescaler 1:64 (pro 20ms periodu)
    PR2 = PWM_PERIOD_TICKS; // Perioda
    TMR2 = 0;               // Reset ?íta?e

    // 2. Nastavení Output Compare 16 (Bod 10a, 10c)
    OC16CON = 0;            // Vypnout OC16
    OC16CONbits.OCTSEL = 0; // Zdroj je Timer 2
    OC16CONbits.OCM = 0b110;// PWM mód bez fault protection
    OC16RS = PWM_MIN_TICKS; // Výchozí st?ída (1 ms)
    OC16R = PWM_MIN_TICKS;  
    
    // 3. Zapnutí
    T2CONbits.ON = 1;       // Zapnout Timer 2
    OC16CONbits.ON = 1;     // Zapnout OC16
    RPE1Rbits.RPE1R = 11; // 11 = OC16 (podle datasheetu PIC32MK)
}
void updatePwm(uint8_t input_val) {
    // Výpo?et ?í?ky pulzu (lineární interpolace)
    // input_val (0-255) -> (938-1875)
    
    uint32_t ticks = PWM_MIN_TICKS + ( (uint32_t)input_val * (PWM_MAX_TICKS - PWM_MIN_TICKS) ) / 255;
    
    OC16RS = ticks; // Aktualizace st?ídy (pro p?í?tí periodu)
}
