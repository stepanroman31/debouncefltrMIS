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
                txMsgNum[0] = 7; 
                integerToBytes(s1_val_int, &txMsgNum[1]);
                integerToBytes(s2_val_int, &txMsgNum[3]);
                sendMessageUSB(txMsgNum, COM_GO);
                break;
            }

            case 3: // CMD(3): Do Table Terminalu
            {
                char buffer[40]; 
                sprintf(buffer, "Pot: %d, S1: %d, S2: %d", potValue, s1_val_int, s2_val_int);

                sendTableTerminalMessageUSB("A1", buffer); 
                break;
            }

            case 0: // CMD(0): Stop
            default:
                break; 
        }
    }
    
    
}

    