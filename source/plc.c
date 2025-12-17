#include "plc.h"
#include "data.h"

// ?asova? pro periodu krok? (nap?. 2s)
static uint16_t plcTimerMs = 0;
#define PLC_STEP_PERIOD_MS 2000

// Detekce náb??ných hran tla?ítek (aby se akce provedla jen jednou p?i stisku)
static bool prev_btnSet = false;
static bool prev_btnTest = false;
static bool prev_btnRun = false;
static bool prev_btnStop = false;
static bool prev_btnReset = false;

void initPLC(void) {
    setPlcState(PLC_PROG);
    setPlcSequenceLength(0);
    setPlcCurrentIndex(0);
    plcTimerMs = 0;
}

// Funkce pro p?epo?et (Zachováno podle va?eho kódu)
uint8_t scaleTo90(uint8_t input255) {
    // (Input * 90) / 255
    uint16_t temp = (uint16_t)input255 * 90;
    return (uint8_t)(temp / 255);
}

uint8_t scaleTo255(uint8_t input90) {
    // (Input * 255) / 90
    if (input90 > 90) input90 = 90;
    uint16_t temp = (uint16_t)input90 * 255;
    return (uint8_t)(temp / 90);
}

void runPLC(bool btnStop, bool btnRun, bool btnReset, bool btnSet, bool btnTest, uint8_t encoderVal) {
    
    plcState_t state = getPlcState();
    uint8_t currentIdx = getPlcCurrentIndex();
    uint8_t seqLen = getPlcSequenceLength();
    
    // Detekce náb??ných hran
    bool rise_Stop = (btnStop && !prev_btnStop);
    bool rise_Run = (btnRun && !prev_btnRun);
    bool rise_Reset = (btnReset && !prev_btnReset);
    bool rise_Set = (btnSet && !prev_btnSet);
    bool rise_Test = (btnTest && !prev_btnTest);

    // --- GLOBÁLNÍ OVLÁDÁNÍ (Priority) ---
    
    // 1. RESET (Nejvy??í priorita - vrátí do PROG a sma?e v?e)
    if (rise_Reset) {
        initPLC(); 
        setPlcState(PLC_PROG); // <--- OPRAVA: Místo PLC_RESET dáváme PLC_PROG
        state = PLC_PROG;
    }
    // 2. STOP (Zastaví b?h)
    else if (rise_Stop) {
        setPlcState(PLC_STOP); // Zápis do globálních dat
        state = PLC_STOP;
    }
    // 3. RUN (Spustí b?h, pokud je sekvence)
    else if (rise_Run && seqLen > 0) {
        setPlcState(PLC_RUN);  // Zápis do globálních dat
        state = PLC_RUN;
        plcTimerMs = 0;        // Reset ?asova?e kroku
    }
    // 4. TEST (Spustí test, pokud je sekvence)
    else if (rise_Test && seqLen > 0) {
        setPlcState(PLC_TEST); // Zápis do globálních dat
        state = PLC_TEST;
        setPlcCurrentIndex(0);
        plcTimerMs = 0;
    }
    
    // --- STAVOVÝ AUTOMAT (Logika uvnit? stav?) ---
    else {
        
        switch (state) {
            
            // --- STAV PROG ---
            case PLC_PROG:
            {
                // 1. P?epo?et a zobrazení hodnoty z kodéru
                uint8_t val90 = scaleTo90(encoderVal);
                setPlcCurrentValue(val90); 
                
                // 2. Tla?ítko SET: Ulo?it do sekvence
                if (rise_Set) {
                    setPlcSequenceValue(seqLen, val90);
                    seqLen++;
                    setPlcSequenceLength(seqLen);
                }
                break;
            }

            // --- STAV TEST ---
            case PLC_TEST:
            {
                // Automatické krokování (jednou)
                plcTimerMs++;
                if (plcTimerMs >= PLC_STEP_PERIOD_MS) {
                    plcTimerMs = 0;
                    currentIdx++;
                    if (currentIdx >= seqLen) {
                        currentIdx = seqLen - 1; // Z?stat na konci
                    }
                    setPlcCurrentIndex(currentIdx);
                }
                
                // Zobrazit hodnotu z pam?ti
                uint8_t val = getPlcSequenceValue(currentIdx);
                setPlcCurrentValue(val);

                // Restart TESTu
                if (rise_Test) { 
                    setPlcCurrentIndex(0);
                    plcTimerMs = 0;
                }
                break;
            }

            // --- STAV RUN ---
            case PLC_RUN:
            {
                // Cyklické krokování
                plcTimerMs++;
                if (plcTimerMs >= PLC_STEP_PERIOD_MS) {
                    plcTimerMs = 0;
                    currentIdx++;
                    if (currentIdx >= seqLen) {
                        currentIdx = 0; // Smy?ka od za?átku
                    }
                    setPlcCurrentIndex(currentIdx);
                }
                
                // Nastavit hodnotu
                uint8_t val = getPlcSequenceValue(currentIdx);
                setPlcCurrentValue(val);
                break;
            }

            // --- STAV STOP ---
            case PLC_STOP:
            {
                // ?asova? stojí, hodnota zamrzlá
                // ?eká se na RUN nebo RESET (?e?eno naho?e v globálním ovládání)
                break;
            }
        }
    }

    // Ulo?ení minulých stav? tla?ítek
    prev_btnSet = btnSet;
    prev_btnTest = btnTest;
    prev_btnRun = btnRun;
    prev_btnStop = btnStop;
    prev_btnReset = btnReset;
}