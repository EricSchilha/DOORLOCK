/*==============================================================================
    DOORLOCK (PIC12F1840) symbolic constants for main and other user functions.
==============================================================================*/

// PORT I/O device definitions

#define BEEPER      LATA0			// Phototransistor light sensor input
#define S1         	RA1			// Temperature sensor (analogue) input
#define LED1      	LATA2			// IR demodulator input
#define MIC			RA3			// Switch S1 input
#define N2      	RA4			// Neopixel2 (J2) output
#define LOCK		LATA5			// Lock output

// Clock frequency for delay macros and simulation

#define _XTAL_FREQ	32000000	// Set clock frequency for time delays
#define FCY	_XTAL_FREQ/4        // Set processor instruction cycle time

// TODO - Add function prototypes for all functions in RAINBO.c here:

void init(void); // Initialization function prototype