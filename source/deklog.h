#ifndef DEK_LOGIC_H
#define DEK_LOGIC_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief P?epo?te hodnotu od potenciometru R1 z rozsahu <-2047, 2047> na rozsah <0, 255>. [cite: 114]
 * @param potValue Surová hodnota z ADC.
 * @return P?epo?tená hodnota 0-255.
 */
uint8_t recalculateR1(int16_t potValue);

/**
 * @brief Implementuje funkci p?epína?e mezi R1 a S9. [cite: 17]
 * @param S2_output Stav LED V2 (true = S9; false = R1). [cite: 119, 120]
 * @param r1_recalc P?epo?tená hodnota z R1 (0-255).
 * @param s9_counter Hodnota ?íta?e S9 (0-255).
 * @return Finální "p?epnutá" hodnota (0-255).
 */
uint8_t runSwitchedOutputLogic(bool S2_output, uint8_t r1_recalc, uint8_t s9_counter);

/**
 * @brief Procedura pro ovládání LED V9 (Min) a V12 (Max) a aktualizaci datového modelu.
 * Svítí V12 p?i 255 a V9 p?i 0. [cite: 121, 122]
 * @param switched_val Finální hodnota (0-255) z p?epína?e.
 */
void runLimitIndicators(uint8_t switched_val);

#endif // DEK_LOGIC_H
