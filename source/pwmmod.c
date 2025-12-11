#include "pwmmod.h"
#include <xc.h>
#include <sys/attribs.h>

#define PWM_PERIOD_TICKS 18749 // 20 ms
#define PWM_MIN_TICKS 938       // 1 ms
#define PWM_MAX_TICKS 1875      // 2 ms

void initPwm(void) {
    // 1. Bezpe?nost: Vypnout p?eru?ení
    __builtin_disable_interrupts();
    TRISEbits.TRISE1 = 0;  
    LATEbits.LATE1 = 0;    // Pro jistotu shoda s nulou na za?átku

    // 2. MAGICKÉ ODEM?ENÍ PIN? (Bez tohoto servo nepojede!)
    SYSKEY = 0xAA996655;     // Klí? 1
    SYSKEY = 0x556699AA;     // Klí? 2
    CFGCONbits.IOLOCK = 0;   // Odemknout zápis

    // 3. P?i?azení pinu (Te? u? to projde)
    //RPE1Rbits.RPE1R = 11;    // Pin RE1 p?ipojit na OC16
    RPB13Rbits.RPB13R = 11;

    // 4. Zamknutí pin?
    CFGCONbits.IOLOCK = 1;   // Zamknout
    SYSKEY = 0;              // Zahodit klí?e

    // 5. Nastavení Timeru 3 (20 ms)
    T3CON = 0;
    T3CONbits.TCKPS = 0b110; // Prescaler 1:64
    PR3 = 18749;             // Perioda
    TMR3 = 0;

    // 6. Nastavení PWM Modulu (OC16)
    OC16CON = 0;
    OC16CONbits.OCTSEL = 1;  // !!! 1 = TIMER 3 !!!
    OC16CONbits.OCM = 0b110; // PWM mód
    OC16RS = 1406;           // Startovní poloha (st?ed)
    OC16R = 1406;

    // 7. Zapnutí
    T3CONbits.ON = 1;
    OC16CONbits.ON = 1;

    // 8. Povolit p?eru?ení
    __builtin_enable_interrupts();
}
void updatePwm(uint8_t input_val) {
    // Výpo?et ?í?ky pulzu (lineární interpolace)
    // input_val (0-255) -> (938-1875)
    
    uint32_t ticks = PWM_MIN_TICKS + ( (uint32_t)input_val * (PWM_MAX_TICKS - PWM_MIN_TICKS) ) / 255;
    
    OC16RS = ticks; // Aktualizace st?ídy (pro p?í?tí periodu)
}
