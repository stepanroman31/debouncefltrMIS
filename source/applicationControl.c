
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
    uint8_t r1_recalc = recalculateR1(potValue_raw);
    runDecoder(S9A_filtered, S9B_filtered); // Aktualizuje globální s9_counte
    uint8_t switched_val = runSwitchedOutputLogic(
        S2_output,     // Stav LED V2 (Pam?? S2 ?ídí multiplexer)
        r1_recalc,     // Hodnota R1 (0-255)
        s9_counter     // Hodnota S9 (0-255)
    );
    uint8_t final_pwm_input;
if (S1_output == true && getRtmCommand() == 4) {
    // LED V1 svítí A povel CMD(4) je aktivní -> Vstup pro PWM bere hodnotu z PC
    final_pwm_input = getRtmParameter();
} else {
    // Standardní ?ízení DEK logikou
    final_pwm_input = runFinalSwitchLogic(S3_output, switched_val); 
}

// --- Nastavení st?ídy PWM ---
updatePwm(final_pwm_input);
    runLimitIndicators(switched_val);
    setSwitchedOutput(final_pwm_input);
    runPLC(S4_filt, S5_filt, S6_filt, S7_filt, S8_filt, s9_counter);
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
    uint8_t plcVal90 = getPlcCurrentValue();
    uint8_t pwmInput255 = scaleTo255(plcVal90);
    if (getButtonS1()) { 
        // KDY? DR?ÍTE S1: Po?leme natvrdo st?ední polohu a rozsvítíme p?lku bargrafu
        updatePwm(128);       
        setFpgaVxValue(128);  
    } else {
        // NORMÁLNÍ PROVOZ: Jedeme podle PLC
        updatePwm(pwmInput255);
        setFpgaVxValue(pwmInput255);
    }
    updatePwm(pwmInput255);
    setFpgaVxValue(final_pwm_input); // Zobrazení hodnoty na LED V13-V24
    runRTMCommunication();
}
  
// runApplication() END)

//--- applicationControl.c file END -------------------------------------------


