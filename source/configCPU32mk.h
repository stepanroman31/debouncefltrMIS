/* 
 * File:   configMk.h
 * Author: jz
 *
 * Created:  2022.03.23
 * Modified: 2023.03.10
 * 
 * Include in programs with HW PIC32MK1024MCF100 (demo board DEP p.m.)
 * 
 */

#ifndef CONFIGMK_H
#define	CONFIGMK_H

// DEVCFG3
// USERID = No Setting
#pragma config PWMLOCK = OFF            // PWM IOxCON lock (PWM IOxCON register writes accesses are not locked or protected)
#pragma config FUSBIDIO2 = OFF          // USB2 USBID Selection (USBID pin is controlled by the port function)
#pragma config FVBUSIO2 = OFF           // USB2 VBUSON Selection bit (VBUSON pin is controlled by the port function)
#pragma config PGL1WAY = ON             // Permission Group Lock One Way Configuration bit (Allow only one reconfiguration)
#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO1 = OFF          // USB1 USBID Selection (USBID pin is controlled by the port function)
#pragma config FVBUSIO1 = OFF           // USB2 VBUSON Selection bit (VBUSON pin is controlled by the port function)

// DEVCFG2
#pragma config FPLLIDIV = DIV_1         // System PLL Input Divider (1x Divider)
#pragma config FPLLRNG = RANGE_8_16_MHZ // System PLL Input Range (8-16 MHz Input)
#pragma config FPLLICLK = PLL_POSC      // System PLL Input Clock Selection (POSC is input to the System PLL)
#pragma config FPLLMULT = MUL_20        // System PLL Multiplier (PLL Multiply by 20)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (2x Divider)
#pragma config VBATBOREN = OFF          // VBAT BOR Enable (Disable ZPBOR during VBAT Mode)
#pragma config DSBOREN = ON             // Deep Sleep BOR Enable (Enable ZPBOR during Deep Sleep Mode)
#pragma config DSWDTPS = DSPS32         // Deep Sleep Watchdog Timer Postscaler (1:2^36)
#pragma config DSWDTOSC = LPRC          // Deep Sleep WDT Reference Clock Selection (Select LPRC as DSWDT Reference clock)
#pragma config DSWDTEN = OFF            // Deep Sleep Watchdog Timer Enable (Disable DSWDT during Deep Sleep Mode)
#pragma config FDSEN = ON               // Deep Sleep Enable (Enable DSEN bit in DSCON)
#pragma config BORSEL = HIGH            // Brown-out trip voltage (BOR trip voltage 2.1v (Non-OPAMP deviced operation))
#pragma config UPLLEN = OFF             // USB PLL Enable (USB PLL Disabled)

// DEVCFG1
#pragma config FNOSC = SPLL             // Oscillator Selection Bits (System PLL)
#pragma config DMTINTV = WIN_127_128    // DMT Count Window Interval (Window/Interval value is 127/128 counter value)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disable Secondary Oscillator)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = EC             // Primary Oscillator Configuration (External clock mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FCKSM = CSECME           // Clock Switching and Monitor Selection (Clock Switch Enabled, FSCM Enabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WDTSPGM = STOP           // Watchdog Timer Stop During Flash Programming (WDT stops during Flash programming)
#pragma config WINDIS = NORMAL          // Watchdog Timer Window Mode (Watchdog Timer is in non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled)
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window size is 25%)
#pragma config DMTCNT = DMT31           // Deadman Timer Count Selection (2^31 (2147483648))
#pragma config FDMTEN = OFF             // Deadman Timer Enable (Deadman Timer is disabled)

// DEVCFG0
#pragma config DEBUG = OFF              // Background Debugger Enable (Debugger is disabled)
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (Communicate on PGEC2/PGED2)
#pragma config TRCEN = OFF              // Trace Enable (Trace features in the CPU are disabled)
#pragma config BOOTISA = MIPS32         // Boot ISA Selection (Boot code and Exception code is MIPS32)
#pragma config FSLEEP = OFF             // Flash Sleep Mode (Flash is powered down when the device is in Sleep mode)
#pragma config DBGPER = PG_ALL          // Debug Mode CPU Access Permission (Allow CPU access to all permission regions)
#pragma config SMCLR = MCLR_NORM        // Soft Master Clear Enable (MCLR pin generates a normal system Reset)
#pragma config SOSCGAIN = GAIN_2X       // Secondary Oscillator Gain Control bits (2x gain setting)
#pragma config SOSCBOOST = ON           // Secondary Oscillator Boost Kick Start Enable bit (Boost the kick start of the oscillator)
#pragma config POSCGAIN = GAIN_LEVEL_3  // Primary Oscillator Gain Control bits (Gain Level 3 (highest))
#pragma config POSCBOOST = ON           // Primary Oscillator Boost Kick Start Enable bit (Boost the kick start of the oscillator)
#pragma config EJTAGBEN = NORMAL        // EJTAG Boot Enable (Normal EJTAG functionality)

// DEVCP
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

// BF1SEQ0
#pragma config TSEQ =       0x0000
#pragma config CSEQ =       0xffff

#include <xc.h>
#include <sys/attribs.h>
#include <stdbool.h>

void configCPU32mk(void) {//----------------------------------------------------
    // CPU Performance Optimization:
    register unsigned long tmp_cache; //KSEG0 cache enable
    asm("mfc0 %0,$16,0" : "=r"(tmp_cache));
    tmp_cache = (tmp_cache & ~7) | 3;
    asm("mtc0 %0,$16,0" ::"r" (tmp_cache));

    CHECONbits.PFMWS = 3;   //Flash wait states = 3 CPU clock cycles @ 120Mhz        
    CHECONbits.PREFEN = 1;  //PREFEN
    PRISSbits.PRI7SS = 1;   //Interrupt with priority level of 7 uses Shadow Set 1
    INTCONbits.MVEC = 1;    //Enable multi-vector interrupts
    __builtin_mtc0(12, 0, (__builtin_mfc0(12, 0) | 0x0001));
     
    /*
    //konfigurace preruseni - XC32 str. 233 - funkcni
    __builtin_disable_interrupts();
    PRISSbits.PRI7SS = 1; //Interrupt with priority level of 7 uses Shadow Set 1
    INTCONbits.MVEC = 1; //multi-vector
    __builtin_enable_interrupts(); 
     */
    __builtin_disable_interrupts();
}// initCPU() END

//--- initCPU.c file END ------------------------------------------------------


#endif // CONFIGMK_H