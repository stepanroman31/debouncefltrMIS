/* 
 * Version: v1.0.2
 * 
 * File: messengerMIS.h
 * 
 * Header for USB, FRAM message transfer functions from lib
 *
 * Created: 2024.06.16 (Based on messengerDEP.h v1.0.2)
 * Modified 2024.06.16
 */ 

//---Constants------------------------------------------------------------------
#define COM_GO    false
#define COM_RESET  true

#define MSG_LEN_IX              0
#define RX_MESSAGE_LENGTH_MASK  0x3F
#define RX_FROM_TEXT_TERMINAL   0x40 
#define RX_FROM__COMMAND_EDITOR 0x00 
#define RX_MESSAGE_TYPE_MASK    0xC0 

//---Communication functions----------------------------------------------------
void initSendGetMessageUART(void);
bool sendMessageUART(
        unsigned char *msg,
        bool reset);
bool getMessageUART(
        unsigned char *msg,
        bool reset);
void sendCharUart(void);
void sendCharUARTn3(void);

void initSendGetMessageUSB(void);
bool sendMessageUSB(
        unsigned char *msg,
        bool reset);
bool getMessageUSB(
        unsigned char *msg,
        bool reset);

void initSendGetMessageFRAM(void);
bool sendMessageFRAM(
        unsigned char *msg,
        unsigned startAddr,
        unsigned lngth,
        bool reset);
bool getMessageFRAM(
        unsigned char *msg,
        unsigned startAddr,
        unsigned lngth,
        bool reset);


//---Variable conversion functions----------------------------------------------
void integerToBytes(
        const signed short intIn,
        unsigned char msgOut[]);
signed short bytesToInteger(
        const unsigned char rxMsg[]);
void floatToBytes(
        const float floatIn,
        unsigned char msgOut[]);
float bytesToFloat(
        const unsigned char rxMsg[]);

//----TableTermina functions (Requires RTM (A67) or later)
bool sendTableTerminalMessageUART(
        char *cellAddr,
        char *cellText);
bool sendTableTerminalMessageUSB(
        char *cellAddr,
        char *cellText);

//---- TextTerminal functions (Requires RTM (A70 or later)
bool sendTextTerminalMessageUART(
        char *text);
bool sendTextTerminalMessageUSB(
        char *text);

//---messengerDEP.h END --------------------------------------------------------
