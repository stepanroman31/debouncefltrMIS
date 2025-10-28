/* 
 * File:   platformMIS32mk.h
 * Author: j.z.
 *
 * Created:  2024.06.16 (Based on platformDEP32mk.h)
 * Modified: 2024.06.16
 */

#ifndef PLATFORMMIS32MK_H
#define	PLATFORMMIS32MK_H
#include <stdbool.h>
//--- Macros ------------------------------------------------------------------
#define NOT !
#define EQUAL ==
#define GREATER_THEN =>
#define LESS_THEN =<
#define NOT_EQUAL !=

#define SET_ADC1_TRIGGER(us) \
  TMR1 = 0;\
  PR1 = (us*60);\
  T1CONbits.ON = 1

// ADC time position measurement support
#define LED_VX PORTCbits.RC7 // LED V5
#define LED_PULSE \
  LED_VX = 0; \
  LED_VX = 1; \
  LED_VX = 0

void setLedPulseActive(bool onOff);
bool getLedPulseActive(void);

#define CLEAR_CAPTURE7_FIFO while (IC7CONbits.ICBNE == 1) \
                              {static unsigned dump; dump = IC7BUF;}

//--- Patform library info
unsigned getPlatformDepVersion(void);
unsigned getPlatformDepDate(void);

//--- User application functions
void configCPU32mk(void);
void configPlatformAutoMode(void);
void configPlatformUserMode(void);
void runPlatform(void);
void configApplication(void);
void runApplication(void);

bool is1ms(void);

bool isUserMode(void);
bool isAutoMode(void);

bool getButtonS1(void);
bool getButtonS2(void);
bool getButtonS3(void);
bool getButtonS4(void);
bool getButtonS5(void);
bool getButtonS6(void);
bool getButtonS7(void);
bool getButtonS8(void);

bool getCoderChannelA(void);
bool getCoderChannelB(void);
bool getCoderButtonSw(void);

void setLedV1(bool onOff);
void setLedV2(bool onOff);
void setLedV3(bool onOff);
void setLedV4(bool onOff);
void setLedV5(bool onOff);
void setLedV6(bool onOff);
void setLedV7(bool onOff);
void setLedV8(bool onOff);

void setCoderLedHL(bool onOff);
void setCoderLedA(bool onOff);
void setCoderLedB(bool onOff);
void setCoderLedLL(bool onOff);

void setLedV13(bool onOff);
void setLedV14(bool onOff);
void setLedV15(bool onOff);
void setLedV16(bool onOff);
void setLedV17(bool onOff);
void setLedV18(bool onOff);
void setLedV19(bool onOff);
void setLedV20(bool onOff);
void setLedV21(bool onOff);
void setLedV22(bool onOff);
void setLedV23(bool onOff);
void setLedV24(bool onOff);

void setFpgaLedV1(bool onOff);
void setFpgaLedV2(bool onOff);
void setFpgaLedV3(bool onOff);
void setFpgaLedV4(bool onOff);
void setFpgaLedV5(bool onOff);
void setFpgaLedV6(bool onOff);
void setFpgaLedV7(bool onOff);
void setFpgaLedV8(bool onOff);
void setFpgaLedV9(bool onOff);
void setFpgaLedV10(bool onOff);
void setFpgaLedV11(bool onOff);
void setFpgaLedV12(bool onOff);
void setFpgaVxValue(unsigned short value);

// Only in USER_MODE
void setSimulatorOn(bool onOff);
void setSimulatorDir(bool onOff);
void setSimulatorUp(bool onOff);
void setSimulatorDown(bool onOff);

// ADC Support
signed short getPotentiometerValue(void);
void setAdcTrigger(int delayMicroSeconds);
signed short getAdc1Value (void);
void setComparator1HighLimit(signed short highLimit);
void setComparator1LowLimit(signed short lowLimit);
bool getConverterVlegErrorFlag(void);
bool getConverterUlegErrorFlag(void);
bool getConverterWlegErrorFlag(void);
void setConverterUlegErrorFlag(bool onOff);
void setConverterVlegErrorFlag(bool onOff);
void setConverterWlegErrorFlag(bool onOff);
bool getConverterError(void);
void setConverterError(bool onOff);

// Input Capture Support
bool getIrcChannelBValue(void);

// PWM Output Compare Support
void setEnableConverter(void);
void setDisableConverter(void);
bool getPwmOutputValue(void);
void setTestPinIRCAasPwmOutput(void);
void setTestPinIRCAasIrcOutput(void);

#endif	/* PLATFORMMIS32MK_H */

