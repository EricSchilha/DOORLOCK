/*==============================================================================
    Project:DOORLOCK
    Version: 1.0				Date: January 25, 2018
    Target: DL-Circuit			Processor: PIC12F1840

 Knock pattern detection that opens and closes a solenoid lock
 =============================================================================*/

#include    "xc.h"              // XC compiler general include file

#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include	"DoorLock.h"			// Include user-created constants and functions
/*==============================================================================
    Beep function. Produces the selected number of half-cycles of a wave with
    a specified time period.
==============================================================================*/

void beep(unsigned char period, unsigned char cycles)
{
	unsigned char i;
	unsigned char t;
	for (i = cycles; i != 0; i --)		// number of beeper pin output flips
	{
		BEEPER = !BEEPER;				// flip beeper pin and
		for (t = period; t != 0; t --);	// wait for period to end
	}
}

int main(void) // Start of program
{
    init(); // Initialize oscillator and I/O Ports
    __delay_us(200);
    LED1 = 1;
    while (1) {
        //beep(1000,500);
        if (S1 == 0) {
            LED1 = 1;
        } else {
            LED1 = 0;
        }
        __delay_us(20);
    }
}
