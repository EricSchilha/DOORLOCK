/*==============================================================================
    DOORLOCK (PIC12F1840) hardware initialization and user functions.
==============================================================================*/

#include	"xc.h"				// XC compiler general include file

#include	"stdint.h"			// Integer definition
#include	"stdbool.h"			// Boolean (true/false) definition

#include	"DoorLock.h"			// For optional user variable definitions

void init(void) {
    // Initialize oscillator

    OSCCON = 0b01101000; // 4MHz

    // Initialize user ports and peripherals:

    OPTION_REG = 0b00010001; // Weak pull-ups on, falling INT interrupt, (set as Timer0)
    //  TMR0 Now 1:16, Watchdog timer is 1:2
    PORTA = 0; // Clear port input registers and output latches
    LATA = 0; // before configuring port pins
    ANSELA = 0b00010000; // Set RA4 (Mic) as analogue input.
    ADCON0 = 0b00000000; // Set up A-D: channel AN0, A-D converter off,
    ADCON1 = 0b01100000; // left justified, FOSC/64 A-D clock, +VDD ref.
    TRISA = 0b00011010; // Set RA1, RA3 and RA4 as digital inputs (RA4 as analogue), RA0, RA2 and RA5 as outputs
    WPUA = 0b00000010; // Enable weak pull-up on RA1 (S1 input)

    WDTCON = 0b00001010; // WDT off, div 1024 (~32ms)

    // TODO - Enable and set up interrupts if needed

    INTCON = 0b00100000;//Enabling interrupts for use with timer, (timer interrupt (PIE1), Global Interrupt and peripheral interrupt)
}
