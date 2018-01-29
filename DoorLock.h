/*==============================================================================
    DOORLOCK (PIC12F1840) symbolic constants for main and other user functions.
==============================================================================*/

// PORT I/O device definitions

#define BEEPER      LATA0		// Buzzer output
#define S1         	RA1			// Switch S1 input
#define LED1      	LATA2       // LED output
//#define MIC		RA3			// Microphone input (currently digital)
//#define IR      	RA4			// IR demodulator input
#define LOCK		LATA5		// Lock output

# define MIC		0b00001100 //(Channel 3 on RA4)

// Clock frequency for delay macros and simulation

#define _XTAL_FREQ	32000000	// Set clock frequency for time delays
#define FCY	_XTAL_FREQ/4        // Set processor instruction cycle time

// TODO - Add function prototypes for all functions in RAINBO.c here:

void init(void); // Initialization function prototype