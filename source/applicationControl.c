
/*
 * File:    applicationControl.c (For MIS)
 * j.z.
 *  
 * MPLABX.IDE v5.10, XC32 v3.00, HW: DEP-PIC32-MK
 * 
 * Created:  2024.06.16
 * Modified: 2024.08.12
 * 
 */

/*==== History trace buffer ===================================================

2024.08.12-Replaced: #include "platformDEP32mk.h" by platformMIS32mk.h
2024.08.09-Mldified: Source code cleaned up.
2024.06.16-New: applicationControl.c TEMPLATE for students MIS 
-------------------------------------------------------------------------------

2023.02.10-New: applicationControl.c TEMPLATE for students DEP 
==== History trace buffer END ================================================*/

//--- Includes ----------------------------------------------------------------
#include <xc.h>
#include <stdbool.h>
#include <sys/attribs.h>
#include "platformMIS32mk.h"
#include "debounce.h"
#include "memory.h"
#include "komunikace.h"
#include "messengerMIS.h"
#include <stdio.h>
#include "dekoder.h"
#include "data.h"
#include "deklog.h"
#include "pwmmod.h"
#include "plc.h"

//-- latform Function prototypes are in "platrformDEP32mk" ---------------------

//--- User function prototypes -------------------------------------------------


//--- Macros ------------------------------------------------------------------


//--- Constants ---------------------------------------------------------------
#define DELAY_ms  500

//--- New variable types ------------------------------------------------------

//--- Global vars -------------------------------------------------------------
// **Globální instance pro funkci FLT (Filtr)**
filterTypeBool_t S1_filter, S2_filter, S9A_filter, S9B_filter, S3_filter;
filterTypeBool_t S4_filter, S5_filter, S6_filter, S7_filter, S8_filter;

// **Globální instance pro funkci PAM?? (Toggle)**
memoryTypeBool_t S1_memory, S2_memory, S3_memory;
extern uint8_t s9_counter;
long inputPeriod;
//--- External vars -----------------------------------------------------------

//---- Functions --------------------------------------------------------------
uint8_t scalePotentiometer_Local(signed short raw_val) {
    int32_t val = raw_val; 
    val = val + 2047;      
    val = val / 16;        
    if (val > 255) val = 255;
    if (val < 0) val = 0;
    return (uint8_t)val;
}

void configApplication(void){//------------------------------------------------
  //--- User defined functions ---
  inputPeriod = 250;
  initFilterTypeBool (&S1_filter, false);
  initFilterTypeBool (&S2_filter, false);
  initFilterTypeBool (&S3_filter, false);
  initFilterTypeBool (&S9A_filter, false);
  initFilterTypeBool (&S9B_filter, false);
  initMemoryTypeBool (&S1_memory, false);
  initMemoryTypeBool (&S2_memory, false);
  initMemoryTypeBool (&S3_memory, false);
  initFilterTypeBool(&S4_filter, false);
  initFilterTypeBool(&S5_filter, false);
  initFilterTypeBool(&S6_filter, false);
  initFilterTypeBool(&S7_filter, false);
  initFilterTypeBool(&S8_filter, false);
  initPLC();
  initDecoder();
  configRTM();
  initPwm();
  setAdcTrigger(1000);
}// configApplication() END 

//===>>>> Call it every 1ms <<<<===

void runApplication(void) {//--------------------------------------------------
 // Získáme ukazatel na na?i centrální "nást?nku"
    int16_t potValue_raw = getPotentiometerValue();
    bool S1_raw = getButtonS1();
    bool S2_raw = getButtonS2();
    bool S3_raw = getButtonS3();
    bool S4_raw = getButtonS4();
    bool S5_raw = getButtonS5();
    bool S6_raw = getButtonS6();
    bool S7_raw = getButtonS7();
    bool S8_raw = getButtonS8();
    // === 1. ?ÁST: ?tení HW a zápis do appState ===
    bool S9A_raw = getCoderChannelA();
    bool S9B_raw = getCoderChannelB();
    
    bool S1_filtered = runFilterTypeBool(&S1_filter, S1_raw);
    bool S2_filtered = runFilterTypeBool(&S2_filter, S2_raw);
    bool S3_filtered = runFilterTypeBool(&S3_filter, S3_raw);
    bool S4_filt = runFilterTypeBool(&S4_filter, S4_raw);
    bool S5_filt = runFilterTypeBool(&S5_filter, S5_raw);
    bool S6_filt = runFilterTypeBool(&S6_filter, S6_raw);
    bool S7_filt = runFilterTypeBool(&S7_filter, S7_raw);
    bool S8_filt = runFilterTypeBool(&S8_filter, S8_raw);
    bool S9A_filtered = runFilterTypeBool(&S9A_filter, S9A_raw);
    bool S9B_filtered = runFilterTypeBool(&S9B_filter, S9B_raw);
    // Ulo?íme filtrované výstupy do appState
    bool S1_output = runMemoryTypeBool(&S1_memory, S1_filtered);
    bool S2_output = runMemoryTypeBool(&S2_memory, S2_filtered);
    bool S3_output = runMemoryTypeBool(&S3_memory, S3_filtered);
    setS1Output(S1_output);
    setS2Output(S2_output);
    setS3Output(S3_output);
// 4. B?h pod?ízených modul?
    // Dekodér (aktualizuje globální s9_counter)
    runDecoder(S9A_filtered, S9B_filtered); 
    
    // PLC Automat (po?ítá logiku sekvence)
    runPLC(S4_filt, S5_filt, S6_filt, S7_filt, S8_filt, s9_counter);


    // === 5. VÝPO?ET CÍLOVÉ HODNOTY (LOGIKA HIERARCHIE) ===
    
    // P?íprava hodnot:
    // A) Potenciometr (p?epo?ítaný na 0-255)
    uint8_t val_Pot = scalePotentiometer_Local(potValue_raw);
    
    // B) PLC (p?epo?ítané na 0-255)
    // (P?edpoklad: PLC vrací 0-90, funkce scaleTo255 to p?evede)
    uint8_t val_PLC = scaleTo255(getPlcCurrentValue());
    
    // Prom?nná pro výsledek
    uint8_t final_pwm_input = 0;

    // --- ROZHODOVÁNÍ PODLE ZADÁNÍ ---
    // Bod 1: S3 (V3) zhasnutá -> PWM signál 1ms (hodnota 0)
    if (S3_output == false) {
        final_pwm_input = 0;
    } 
    else {
        // Bod 2: S3 (V3) svítí -> Rozhoduje S2 (V2)
        
        if (S2_output == false) {
            // S2 (V2) zhasnutá -> ?ízení Potenciometrem R1
            final_pwm_input = val_Pot;
        } 
        else {
            // S2 (V2) svítí -> ?ízení z PLC (které pou?ívá Kodér S9)
            final_pwm_input = val_PLC;
        }
    }

    // === 6. PRIORITA PC (CMD 4) ===
    // Pokud svítí V1 (S1_output) a p?ijde CMD 4, p?epí?e se v?e.
    if (S1_output == true && getRtmCommand() == 4) {
        int rawVal = getRtmParameter();
        
        // Omezení (Clamping)
        if (rawVal > 255) rawVal = 255;
        if (rawVal < 0) rawVal = 0;
        
        final_pwm_input = (uint8_t)rawVal;
    }


    // === 7. AKTUALIZACE VÝSTUP? ===
    
    // PWM Motor (p?es novou bezpe?nou funkci s p?eru?ením)
    updatePwm(final_pwm_input);
    
    // Indikace limit? (V9=0, V12=255)
    setLedV9(final_pwm_input == 0);
    setLedV12(final_pwm_input == 255);
    
    // Bargraf (FPGA)
    setFpgaVxValue(final_pwm_input);
    
    // LED Stavy tla?ítek
    setLedV1(S1_output); 
    setLedV2(S2_output); 
    setLedV3(S3_output);
    setLedV4(S4_filt);
    setLedV5(S5_filt);
    setLedV6(S6_filt);
    setLedV7(S7_filt);
    setLedV8(S8_filt);
    setCoderLedA(S9A_filtered); 
    setCoderLedB(S9B_filtered); 

    // Komunikace
    runRTMCommunication();
}
  
// runApplication() END)

//--- applicationControl.c file END -------------------------------------------


