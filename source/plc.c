#include "plc.h"
#include "data.h"

// ?asova? pro 2s periodu 
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

uint8_t scaleTo90(uint8_t input255) {
    // (Input * 90) / 255
    return (uint8_t)((input255 * 90) / 255);
}

uint8_t scaleTo255(uint8_t input90) {
    // (Input * 255) / 90
    if (input90 > 90) input90 = 90;
    return (uint8_t)((input90 * 255) / 90);
}

void runPLC(bool btnStop, bool btnRun, bool btnReset, bool btnSet, bool btnTest, uint8_t encoderVal) {
    
    plcState_t state = getPlcState();
    uint8_t currentIdx = getPlcCurrentIndex();
    uint8_t seqLen = getPlcSequenceLength();
    
    // Náb??né hrany tla?ítek
    bool rise_Stop = (btnStop && !prev_btnStop);
    bool rise_Run = (btnRun && !prev_btnRun);
    bool rise_Reset = (btnReset && !prev_btnReset);
    bool rise_Set = (btnSet && !prev_btnSet);
    bool rise_Test = (btnTest && !prev_btnTest);
// Globální RESET (Funguje ve v?ech stavech) [cite: 371, 386, 392, 397]
    if (rise_Reset) {
        initPLC(); // Vyma?e sekvenci a jde do PROG
        state = PLC_PROG;
    } else {
        
        switch (state) {// --- STAV PROG --- [cite: 363]
            case PLC_PROG:
            {
                // 1. Nastavujeme hodnotu kodérem (0-90)
                uint8_t val90 = scaleTo90(encoderVal);
                setPlcCurrentValue(val90); // Zobrazujeme live hodnotu
                
                // 2. Tla?ítko SET: Ulo?it do sekvence
                if (rise_Set) {
                    setPlcSequenceValue(seqLen, val90);
                    seqLen++;
                    setPlcSequenceLength(seqLen);
                    // Zde by mohla být indikace ulo?ení (bliknutí)
                }

                // 3. P?echod do TEST [cite: 367]
                if (rise_Test && seqLen > 0) {
                    setPlcState(PLC_TEST);
                    setPlcCurrentIndex(0);
                    plcTimerMs = 0;
                }
                // 4. P?echod do RUN [cite: 368]
                if (rise_Run && seqLen > 0) {
                    setPlcState(PLC_RUN);
                    setPlcCurrentIndex(0);
                    plcTimerMs = 0;
                }
                break;
            }

            // --- STAV TEST --- [cite: 372]
            case PLC_TEST:
            {
                // Automatické krokování (jednou)
                plcTimerMs++;
                if (plcTimerMs >= PLC_STEP_PERIOD_MS) {
                    plcTimerMs = 0;
                    currentIdx++;
                    if (currentIdx >= seqLen) {
                        currentIdx = seqLen - 1; // Z?stat na konci [cite: 373]
                    }
                    setPlcCurrentIndex(currentIdx);
                }
                
                // Zobrazit hodnotu z pam?ti
                uint8_t val = getPlcSequenceValue(currentIdx);
                setPlcCurrentValue(val);

                // Tla?ítka
                if (rise_Test) { // Restart TESTu [cite: 375]
                    setPlcCurrentIndex(0);
                    plcTimerMs = 0;
                }
                if (rise_Run) { // Jít do RUN [cite: 376]
                    setPlcState(PLC_RUN);
                    setPlcCurrentIndex(0);
                    plcTimerMs = 0;
                }
                break;
            }

            // --- STAV RUN --- [cite: 387]
            case PLC_RUN:
            {
                // Cyklické krokování
                plcTimerMs++;
                if (plcTimerMs >= PLC_STEP_PERIOD_MS) {
                    plcTimerMs = 0;
                    currentIdx++;
                    if (currentIdx >= seqLen) {
                        currentIdx = 0; // Smy?ka od za?átku [cite: 405]
                    }
                    setPlcCurrentIndex(currentIdx);
                }
                
                // Nastavit hodnotu
                uint8_t val = getPlcSequenceValue(currentIdx);
                setPlcCurrentValue(val);

                // Tla?ítka
                if (rise_Stop) { // Jít do STOP [cite: 390]
                    setPlcState(PLC_STOP);
                }
                break;
            }

            // --- STAV STOP --- [cite: 393]
            case PLC_STOP:
            {
                // ?asova? stojí, hodnota zamrzlá
                // Tla?ítka
                if (rise_Run) { // Zp?t do RUN (pokra?ovat) [cite: 396]
                    setPlcState(PLC_RUN);
                }
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
