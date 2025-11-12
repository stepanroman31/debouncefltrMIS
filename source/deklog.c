// dek_logic.c
#include "deklog.h"
#include "platformMIS32mk.h" // Pro setCoderLedLL/HL
#include "appStateLibrary.h" // NOV?: Pro zápis stavu V9/V12

// Funkce recalculateR1 a getSwitchedOutput Z?STÁVAJÍ STEJNÉ (jsou to jen výpo?ty)
uint8_t recalculateR1(int16_t potValue) {
    // Definice funkce, kterou linker hledá!
    
    // P?epo?et <-2047, 2047> na <0.0, 1.0>
    float normalized = ((float)potValue + 2047.0) / 4094.0;
    
    // Omezení (clipping) pro bezpe?nost
    if (normalized < 0.0) normalized = 0.0;
    if (normalized > 1.0) normalized = 1.0;
    
    // P?epo?et <0.0, 1.0> na <0, 255>
    return (uint8_t)(normalized * 255.0);
}

// Tuto funkci upravíme, aby zapisovala do appState
void runLimitIndicators(uint8_t switched_val) {
    // Získáme ukazatel na databázi
    app_state_t *app_state = get_app_state_address();

    bool led_v9_state = (switched_val == 0);
    bool led_v12_state = (switched_val == 255);

    // 1. Nastavení hardwarových LED
    setCoderLedLL(led_v9_state);   // LED V9 (Min)
    setCoderLedHL(led_v12_state);  // LED V12 (Max)

    // 2. Aktualizace databáze appState (pro RTM)
    // POZN: appState nemá sloty pro V9/V12, pou?ijeme sloty v decoder_0
    app_state->decoder_0.isMin = led_v9_state;
    app_state->decoder_0.isMax = led_v12_state;
    
    // Ulo?íme i finální hodnotu (pro CMD 3)
    app_state->pwm_0.input = switched_val; // (pwm_0.input je dobré místo pro finální hodnotu 0-255)
}
