// dek_logic.c
// dek_logic.c
#include "deklog.h" 
#include "data.h"          // Pouzivame settery odsud
#include "platformMIS32mk.h" 
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

// --- Funkce P?epo?tu R1 (Bod 3) ---
uint8_t recalculateR1(int16_t potValue) {
    // P?epo?et z rozsahu <-2047, 2047> na <0, 255>
    float normalized = ( (float)potValue + 2047.0f ) / 4094.0f; 
    
    // Zaji?t?ní, ?e hodnota je v rozsahu <0, 1>
    if (normalized < 0.0f) normalized = 0.0f;
    if (normalized > 1.0f) normalized = 1.0f;
    
    // P?evod na cílový rozsah <0, 255> s explicitním zaokrouhlením
    return (uint8_t)(normalized * 255.0f + 0.5f);
}

// --- Funkce P?epína?e (Multiplexer) (Bod 5) ---
uint8_t runSwitchedOutputLogic(bool S2_output, uint8_t r1_recalc, uint8_t s9_counter) {
    if (S2_output == true) { // LED V2 svítí -> Pou?ij ?íta? S9
        return s9_counter;
    } else { // LED V2 nesvítí -> Pou?ij Potenciometr R1
        return r1_recalc;
    }
}

// --- Funkce Limit? (V9/V12) (Bod 6) ---
void runLimitIndicators(uint8_t switched_val) {
    
    bool led_v9_state = (switched_val == 0);   // Min limit
    bool led_v12_state = (switched_val == 255); // Max limit

    // 1. Nastavení hardwarových LED V9 a V12 (Fyzická akce)
    setCoderLedLL(led_v9_state); 
    setCoderLedHL(led_v12_state); 

    // 2. Aktualizace datového modelu (pro RTM)
    setLedV9(led_v9_state);
    setLedV12(led_v12_state);
    setSwitchedOutput(switched_val); 
}
