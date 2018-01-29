/*==============================================================================
    DOORLOCK (PIC12F1840) hardware initialization and user functions.
==============================================================================*/

#include	"xc.h"				// XC compiler general include file

#include	"stdint.h"			// Integer definition
#include	"stdbool.h"			// Boolean (true/false) definition

#include	"DoorLock.h"			// For optional user variable definitions

void init(void) {
    // Initialize oscillator

    OSCCON = 0b11110000; // 4xPLL with 8 MHz internal oscillator = 32 MHz
    while (!PLLR); // Wait until PLL is locked for timing

    // Initialize user ports and peripherals:

    OPTION_REG = 0b00010001; // Weak pull-ups on, falling INT interrupt, (set as Timer0)
//     TRM0 internal, 1:2 pre-scaler    //Now 1:4, Watchdog timer is also at 1:2 now I think  but its not enabled anyway
    PORTA = 0; // Clear port input registers and output latches
    LATA = 0; // before configuring port pins
    ANSELA = 0b00010000; // Set RA4 (Mic) as analogue input.
    ADCON0 = 0b00000000; // Set up A-D: channel AN0, A-D converter off,
    ADCON1 = 0b01100000; // left justified, FOSC/64 A-D clock, +VDD ref.
    TRISA = 0b00011010; // Set RA1, RA3 and RA4 as digital inputs (RA4 as analogue), RA0, RA2 and RA5 as outputs
    WPUA = 0b00000010; // Enable weak pull-up on RA1 (S1 input)

    WDTCON = 0b00001010; // WDT off, div 1024 (~32ms)

    // TODO - Enable and set up interrupts if needed

    INTCON = 0b11100000;//0; // (Keep interrupts disabled for now) //Enabling interrupts for use with timer, (timer interrupt (PIE1), Global Interrupt and peripheral interrupt)
}



/*
https://learn.adafruit.com/secret-knock-activated-drawer-lock/code


there are some timer modules if you look in the datasheet for the chip
i think it starts on page 143
but idk how to do any of it so ill be no help
that's all i can offer


http://www.microcontrollerboard.com/pic-timer0-tutorial.html
http://ww1.microchip.com/downloads/en/DeviceDoc/51682A.pdf
https://circuitdigest.com/microcontroller-projects/pic-microcontroller-timer-tutorial
just try googling "creating a timer in pic microcontroller".
 */