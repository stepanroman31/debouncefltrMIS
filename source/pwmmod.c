#include "pwmmod.h"
#include <xc.h>
#include <sys/attribs.h>

#define PWM_PERIOD_TICKS 18749 // 20 ms
#define PWM_MIN_TICKS 938       // 1 ms
#define PWM_MAX_TICKS 1875      // 2 ms
#define PWM_RANGE        (PWM_MAX_TICKS - PWM_MIN_TICKS)

volatile uint8_t g_pwm_request = 0;

void initPwm(void) {
     T2CON = 0;               // Reset Timeru 2
    T2CONbits.TCKPS = 0b110; // Prescaler 1:64
    PR2 = PWM_PERIOD_TICKS;  // Perioda 20ms (18750)
    TMR2 = 0;                // Vynulovat ?íta?

   OC16CON = 0;             // Reset
    
   
    OC16CONbits.OCTSEL = 0;  
    
    OC16CONbits.OCM = 0b110; // PWM mód bez fault protection
    OC16RS = 1406;           // Startovní poloha (st?ed 1.5ms)
    OC16R = 1406;

    IPC2bits.T2IP = 1;       // Priorita 1
    IPC2bits.T2IS = 3;       // Subpriorita 3
    IFS0bits.T2IF = 0;       // Smazat vlajku
    IEC0bits.T2IE = 1;       // Povolit p?eru?ení Timeru 2

    T2CONbits.ON = 1;        // Start Timer 2
    OC16CONbits.ON = 1;      // Start OC16
}

// Funkce jen ulo?í po?adavek. Zápis do hardwaru ud?lá a? p?eru?ení.
void updatePwm(uint8_t input_val) {
    g_pwm_request = input_val;
}

// Obsluha p?eru?ení Timeru 2 (ka?dých 20 ms)
void __attribute__((interrupt(IPL1SOFT), vector(_TIMER_2_VECTOR))) Timer2Handler(void) {
    
    // 1. Na?íst hodnotu
    uint8_t val = g_pwm_request;

    // 2. Výpo?et ?í?ky pulzu
    // Vzorec: Min + (Hodnota * Rozsah) / 255
    uint32_t ticks = PWM_MIN_TICKS + ( (uint32_t)val * PWM_RANGE ) / 255;

    // 3. Zápis do registru
    OC16RS = (uint16_t)ticks;

    // 4. Smazat vlajku p?eru?ení
    IFS0bits.T2IF = 0;
}
