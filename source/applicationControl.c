
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
#include "appStateLibrary.h"
#include "deklog.h"

//-- latform Function prototypes are in "platrformDEP32mk" ---------------------

//--- User function prototypes -------------------------------------------------


//--- Macros ------------------------------------------------------------------


//--- Constants ---------------------------------------------------------------
#define DELAY_ms  500

//--- New variable types ------------------------------------------------------

//--- Global vars -------------------------------------------------------------
// **Globální instance pro funkci FLT (Filtr)**
filterTypeBool_t S1_filter, S2_filter, S9A_filter, S9B_filter, S3_filter;

// **Globální instance pro funkci PAM?? (Toggle)**
memoryTypeBool_t S1_memory, S2_memory, S3_memory;
extern uint8_t s9_counter;
long inputPeriod;
//--- External vars -----------------------------------------------------------

//---- Functions --------------------------------------------------------------

void configApplication(void){//------------------------------------------------
  //--- User defined functions ---
  init_app_state();
  inputPeriod = 250;
  initFilterTypeBool (&S1_filter, false);
  initFilterTypeBool (&S2_filter, false);
  initFilterTypeBool (&S3_filter, false);
  initFilterTypeBool (&S9A_filter, false);
  initFilterTypeBool (&S9B_filter, false);
  initMemoryTypeBool (&S1_memory, false);
  initMemoryTypeBool (&S2_memory, false);
  initMemoryTypeBool (&S3_memory, false);
  initDecoder();
  configRTM();
}// configApplication() END 

//===>>>> Call it every 1ms <<<<===

void runApplication(void) {//--------------------------------------------------
 // Získáme ukazatel na na?i centrální "nást?nku"
    app_state_t *app_state = get_app_state_address();

    // === 1. ?ÁST: ?tení HW a zápis do appState ===
    app_state->button_s1.filter.input = getButtonS1();
    app_state->button_s2.filter.input = getButtonS2();
    app_state->button_s3.filter.input = getButtonS3();
    app_state->adc_0.inputUnformatted = getPotentiometerValue();
    bool S9A_raw = getCoderChannelA();
    bool S9B_raw = getCoderChannelB();
    bool S1_filtered = runFilterTypeBool(&S1_filter, app_state->button_s1.filter.input);
    bool S2_filtered = runFilterTypeBool(&S2_filter, app_state->button_s2.filter.input);
    bool S3_filtered = runFilterTypeBool(&S3_filter, app_state->button_s3.filter.input);
    bool S9A_filtered = runFilterTypeBool(&S9A_filter, S9A_raw);
    bool S9B_filtered = runFilterTypeBool(&S9B_filter, S9B_raw);
    // Ulo?íme filtrované výstupy do appState
    app_state->decoder_0.outAFiltered = S9A_filtered;
    app_state->decoder_0.outBFiltered = S9B_filtered;
    app_state->button_s1.outputMemory = runMemoryTypeBool(&S1_memory, S1_filtered);
    app_state->button_s2.outputMemory = runMemoryTypeBool(&S2_memory, S2_filtered);
    app_state->button_s3.outputMemory = runMemoryTypeBool(&S3_memory, S3_filtered);
    runDecoder(S9A_filtered, S9B_filtered); // Aktualizuje globální s9_counter
    app_state->decoder_0.outputInBaseRange = s9_counter;
    app_state->adc_0.outputInBaseRange = recalculateR1(app_state->adc_0.inputUnformatted);
    uint8_t switched_val = getSwitchedOutput(
        app_state->button_s2.outputMemory, 
        app_state->adc_0.outputInBaseRange, 
        app_state->decoder_0.outputInBaseRange
    );
    runLimitIndicators(switched_val);
    setLedV1(app_state->button_s1.outputMemory); 
    setLedV2(app_state->button_s2.outputMemory); 
    setCoderLedA(S9A_filtered); 
    setCoderLedB(S9B_filtered); 
    setFpgaVxValue(switched_val); // Zobrazení hodnoty na LED V13-V24
    runRTMCommunication();
}
  
// runApplication() END)

//--- applicationControl.c file END -------------------------------------------


