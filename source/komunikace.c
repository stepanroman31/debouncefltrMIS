#include "komunikace.h"
#include "data.h"
#include "messengerMIS.h"
#include <stdio.h>

#define RTM_SEND_PERIOD_MS 40 
#define MSG_MAX_NUM 17 
#define RTM_RX_INTEGER_MSG_LENGTH 7 
#define COM_GO false

// === GLOBALNI STAVY PRO RTM MODUL ===
signed short rtmCommand = 0;
static unsigned short cntPerformCom = 0; 

static unsigned char rxMsg[MSG_MAX_NUM]; 
static unsigned char txMsgNum[MSG_MAX_NUM];
static unsigned char cmd3_state = 0;

// --- Inicializace (Volaná jednou z configApplication) ---
void configRTM(void) {
    initSendGetMessageUSB(); 
}
// --- Hlavní funkce RTM (Volaná ka?dou 1 ms z runApplication) ---
void runRTMCommunication(void) {
    
    // 1. PRIJEM POVELU (Dekódování CMD z datových bajt?)
    if (getMessageUSB(rxMsg, COM_GO) == true) {
        
        // Zpracujeme jen, pokud zpráva má správnou délku a typ (INT Command)
        if ( (rxMsg[0] & RX_MESSAGE_LENGTH_MASK) == RTM_RX_INTEGER_MSG_LENGTH &&
             (rxMsg[MSG_LEN_IX] & RX_MESSAGE_TYPE_MASK) == RX_FROM__COMMAND_EDITOR ) 
        {
            // Povel je int16_t, za?íná na indexu [1]
            signed short receivedCmd = bytesToInteger(&rxMsg[1]);
            
            if (receivedCmd >= 0 && receivedCmd <= 3) {
                rtmCommand = receivedCmd; 
            }
        }
    }
    // 2. ODESILANI DAT (perioda 40 ms)
    if (cntPerformCom++ >= RTM_SEND_PERIOD_MS) {
        cntPerformCom = 0;
        
        // Získání dat z datového modelu
        int16_t potValue = getPotValue();
        int16_t s1_val_int = getS1Output() ? 1 : 0;
        int16_t s2_val_int = getS2Output() ? 1 : 0;
        
        switch (rtmCommand) {
            
            case 1: // CMD(1): Potenciometr do grafu (1x int16_t)
            {
                txMsgNum[0] = 7; 
                integerToBytes(potValue, &txMsgNum[1]); 
                sendMessageUSB(txMsgNum, COM_GO); 
                break;
            }

            case 2: // CMD(2): S1 a S2 do grafu (2x int16_t)
            {
                txMsgNum[0] = 5; 
                integerToBytes(s1_val_int, &txMsgNum[1]);
                integerToBytes(s2_val_int, &txMsgNum[3]);
                sendMessageUSB(txMsgNum, COM_GO);
                break;
            }

            case 3: // CMD(3): Do Table Terminalu
            {
                char buffer[40]; 
                switch (cmd3_state) 
                {
                    case 0: // Stav 0: Poslat Potenciometr
                        sprintf(buffer, "POT: %d", getPotValue()); // Naformátuj jen ?íslo
                        sendTableTerminalMessageUSB("1A", buffer); // Po?li do bu?ky A1 
                        break;
                        
                    case 1: // Stav 1: Poslat S1
                        sprintf(buffer, "S1: %d", getS1Output() ? 1 : 0); // Naformátuj jen ?íslo
                        sendTableTerminalMessageUSB("1B", buffer); // Po?li do bu?ky B1
                        break;
                        
                    case 2: // Stav 2: Poslat S2
                        sprintf(buffer, "S2: %d", getS2Output() ? 1 : 0); // Naformátuj jen ?íslo
                        sendTableTerminalMessageUSB("1C", buffer); // Po?li do bu?ky C1
                        break;
                }
                
                // Posun na dal?í stav pro p?í?tí 40ms cyklus
                cmd3_state++;
                if (cmd3_state > 2) { // Pokud jsme byli ve stavu 2, vrátíme se na 0
                    cmd3_state = 0;
                }

                break; // Konec case 3
            }
                break;
            

            case 0: // CMD(0): Stop
            default:
                break; 
        }
    }
    
    
}

    