// dek_decoder.c
#include "dekoder.h"

// Definice globálního ?íta?e
uint8_t s9_counter = 0;

// Statická prom?nná pro ulo?ení minulého stavu signál?
static uint8_t last_state = 0; // 00, 01, 10, 11

void initDecoder(void) {
    s9_counter = 0;
    last_state = 0; // P?edpokládáme start na 00
}

// Tato funkce musí být volána periodicky (ka?dou 1ms)
void runDecoder(bool A_filtered, bool B_filtered) {
    
    // P?evedeme dva bool stavy na 2-bitové ?íslo (0, 1, 2, nebo 3)
    uint8_t current_state = (A_filtered << 1) | B_filtered;

    // Zm?na nastala pouze pokud se stav li?í od minulého
    if (current_state != last_state) {
        
        // Logika pro ur?ení sm?ru (CW = po sm?ru hod. ru?i?ek)
        // CW p?echody: 00->01, 01->11, 11->10, 10->00
        if ( (last_state == 0 && current_state == 1) ||
             (last_state == 1 && current_state == 3) ||
             (last_state == 3 && current_state == 2) ||
             (last_state == 2 && current_state == 0) ) 
        {
            // Sm?r CW: Inkrementace (s kontrolou p?ete?ení)
            if (s9_counter < 255) {
                s9_counter++;
            }
        }
        // CCW p?echody: 00->10, 10->11, 11->01, 01->00
        else if ( (last_state == 0 && current_state == 2) ||
                  (last_state == 2 && current_state == 3) ||
                  (last_state == 3 && current_state == 1) ||
                  (last_state == 1 && current_state == 0) )
        {
            // Sm?r CCW: Dekrementace (s kontrolou podte?ení)
            if (s9_counter > 0) {
                s9_counter--;
            }
        }
        
        // Ulo?ení aktuálního stavu pro p?í?tí cyklus
        last_state = current_state;
    }
}
