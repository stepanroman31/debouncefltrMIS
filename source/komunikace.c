#include "komunikace.h"
#include "data.h"
#include "messengerMIS.h"
#include <stdio.h>
#include "deklog.h"
#include <stdint.h>
#include <stdbool.h>
#include "pwmmod.h"


#define RTM_SEND_PERIOD_MS 50 
#define MSG_MAX_NUM 17 
#define RTM_RX_INTEGER_MSG_LENGTH 7 
#define COM_GO false

// === GLOBALNI STAVY PRO RTM MODUL ===
static unsigned short cntPerformCom = 0; 

static unsigned char rxMsg[MSG_MAX_NUM]; 
static unsigned char txMsgNum[MSG_MAX_NUM];
static unsigned char cmd3_state = 0;
signed short rtmCommand = 0;

// --- Inicializace (Volaná jednou z configApplication) ---
void configRTM(void) {
    initSendGetMessageUSB(); 
}
// --- Hlavní funkce RTM (Volaná ka?dou 1 ms z runApplication) ---
void runRTMCommunication(void) {
    // 1. PRIJEM POVELU (Dekódování CMD z datových bajt?)
    if (getMessageUSB(rxMsg, COM_GO) == true) {
        
        // Zpracujeme jen, pokud zpráva má správnou délku a typ (INT Command)
        if ( (rxMsg[0] & RX_MESSAGE_LENGTH_MASK) == RTM_RX_INTEGER_MSG_LENGTH  &&
             (rxMsg[MSG_LEN_IX] & RX_MESSAGE_TYPE_MASK) == RX_FROM__COMMAND_EDITOR ) 
        {
            // Povel je int16_t, za?íná na indexu [1]
            signed short receivedCmd = bytesToInteger(&rxMsg[1]);
            signed short param1 = bytesToInteger(&rxMsg[3]);
           if(param1 > 255){
                    param1 = 255;
                }
           if(param1 < 0){
                    param1 = 0;
            }
            
            if (receivedCmd >= 0 && receivedCmd <= 4) {
                setRtmCommand(receivedCmd);
            }
            if (receivedCmd == 4) {
                // P?ekontrolujeme limity (0-255) a ulo?íme hodnotu z PC
                if (param1 >= 0 && param1 <= 255) {
                    setRtmParameter((uint8_t)param1);// Ulo?ení hodnoty z PC pro pou?ití v runApplication
                }
        }
    }
    }
    // 2. ODESILANI DAT (perioda 50 ms)
        
    if (cntPerformCom++ >= 50 ) {
        cntPerformCom = 0;
        
        // Získání dat z datového modelu
        signed short activeCmd = getRtmCommand();
        uint8_t switched_val = getSwitchedOutput();
        int16_t s1_val_int = getS1Output() ? 1 : 0;
        int16_t s2_val_int = getS2Output() ? 1 : 0;
        int16_t s3_val_int = getS3Output() ? 1 : 0;
        int16_t v9_val_int = getLedV9() ? 1 : 0;
        int16_t v12_val_int = getLedV12() ? 1 : 0;
        
        switch (activeCmd) {
            
            case 1: // CMD(1): Potenciometr do grafu (1x int16_t)
            {
                txMsgNum[0] = 7; 
                integerToBytes((int16_t)switched_val, &txMsgNum[1]);
                integerToBytes(v9_val_int, &txMsgNum[3]);
                integerToBytes(v12_val_int, &txMsgNum[5]);
                sendMessageUSB(txMsgNum, COM_GO); 
                break;
            }

            case 2: // CMD(2): S1 a S2 do grafu (2x int16_t)
            {
                txMsgNum[0] = 7; 
                integerToBytes(s1_val_int, &txMsgNum[1]);
                integerToBytes(s2_val_int, &txMsgNum[3]);
                integerToBytes(s3_val_int, &txMsgNum[5]);
                sendMessageUSB(txMsgNum, COM_GO);
                break;
            }

            case 3: // CMD(3): Do Table Terminalu
            {
                char buffer[20]; 
                uint8_t idx = getPlcCurrentIndex();      // Polo?ka 1B
                uint8_t val90 = getPlcCurrentValue();    // Polo?ka 1C
                plcState_t aktualniStav = getPlcState();
                
                // 2. P?ipravíme si prom?nnou pro text
                char *textProTerminal = "Err";

                // 3. IF podmínky pro výb?r textu
                if (aktualniStav == PLC_PROG) {
                    textProTerminal = "Prog";  // Kdy? byl Reset (S6)
                } 
                else if (aktualniStav == PLC_RUN) {
                    textProTerminal = "Run";   // Kdy? byl Run (S5)
                } 
                else if (aktualniStav == PLC_STOP) {
                    textProTerminal = "Stop";  // Kdy? byl Stop (S4)
                } 
                else if (aktualniStav == PLC_TEST) {
                    textProTerminal = "Test";  // Kdy? byl Test (S8)
                }

                // 3. Odesílání po ?ástech (stavový automat pro komunikaci)
                switch (cmd3_state) 
                {
                    case 0: // Bu?ka 1A: Text stavu (nap?. "Prog")
                        sendTableTerminalMessageUSB("1A", textProTerminal); 
                        break;
                        
                    case 1: // Bu?ka 1B: Index kroku (nap?. "0", "1", "2"...)
                        sprintf(buffer, "%d", idx);
                        sendTableTerminalMessageUSB("1B", buffer);
                        break;
                        
                    case 2: // Bu?ka 1C: Hodnota zat??ovatele (0-90)
                        sprintf(buffer, "%d", val90);
                        sendTableTerminalMessageUSB("1C", buffer);
                        break;
                }
                
                // Posun na dal?í bu?ku pro p?í?tí cyklus (máme jen 3 bu?ky: 0, 1, 2)
                cmd3_state++;
                if (cmd3_state > 3) { 
                    cmd3_state = 0;
                }
                break;
            }
            break;
            case 4:
            {
                // Vstup do modulátoru (p?ijat z PC, ale odesíláme ho zp?t pro vizualizaci)
                uint8_t rtm_input = getRtmParameter(); 
                
                // Zpráva pro 1x int16_t = délka 3 bajty
                txMsgNum[0] = 3; 
                integerToBytes((int16_t)rtm_input, &txMsgNum[1]);
                sendMessageUSB(txMsgNum, COM_GO);
                break;
            }

            case 0: // CMD(0): Stop
            default:
                break; 
        }
    }
    
    
}