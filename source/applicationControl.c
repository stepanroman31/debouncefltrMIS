
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
#include "messengerMIS.h"

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
static int16_t rtmCommand = 0;
static uint32_t sendTimer = 0;
long inputPeriod;
//--- External vars -----------------------------------------------------------

//---- Functions --------------------------------------------------------------

void configApplication(void){//------------------------------------------------
  //--- User defined functions ---
  inputPeriod = 250;
  initFilterTypeBool (&S1_filter, false);
  initFilterTypeBool (&S2_filter, false);
  initFilterTypeBool (&S9A_filter, false);
  initFilterTypeBool (&S9B_filter, false);
  initMemoryTypeBool (&S1_memory, false);
  initMemoryTypeBool (&S2_memory, false);

}// configApplication() END 

//===>>>> Call it every 1ms <<<<===

void runApplication(void) {//--------------------------------------------------
    bool S1_raw = getButtonS1();
    bool S2_raw = getButtonS2();
    bool S9A_raw = getCoderChannelA();
    bool S9B_raw = getCoderChannelB();
    bool S1_filtered = runFilterTypeBool(&S1_filter, S1_raw);
    bool S2_filtered = runFilterTypeBool(&S2_filter, S2_raw);
    bool S9A_filtered = runFilterTypeBool(&S9A_filter, S9A_raw);
    bool S9B_filtered = runFilterTypeBool(&S9B_filter, S9B_raw);
    bool S1_output = runMemoryTypeBool(&S1_memory, S1_filtered);
    bool S2_output = runMemoryTypeBool(&S2_memory, S2_filtered);
    setCoderLedA(S9A_filtered);
    setCoderLedB(S9B_filtered);
    setLedV1(S1_output);
    setLedV2(S2_output);
}
  
// runApplication() END)

//--- applicationControl.c file END -------------------------------------------


