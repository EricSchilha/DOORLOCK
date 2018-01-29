/*==============================================================================
    Project:    DOORLOCK
    Version:    1.0				Date:       January 25-28, 2018
    Target:     DL-Circuit		Processor:  PIC12F1840

 Knock pattern detection that opens and closes a solenoid lock
 =============================================================================*/

/*============================================================================
 * 
 *                          POTENTIAL ISSUES
 * 
 *  -checkTimerInterrupt() method is not called automatically (FIXED)
 *  -the calling of checkTimerInterrupt() exits the currently running method (FIXED)
 *  -sensitivity variable is not set accurately (will require testing)
 *  -the microphone will pick up sound other than the knock
 *  -multiple knocks will be recorded when running the loop, with 0 time in between (FIXED)
 * 
 */

#include    "xc.h"              // XC compiler general include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions
#include	"DoorLock.h"		// Include user-created constants and functions

unsigned char codeLength = 3;                                                   //Length of code.
unsigned char code[3] = {2, 2, 0};//, 0};                                  //Stores the code. Potential upgrade could be to allow the user to add this. 1 is a quarter note, which is 408ms, but didn't adjust properly so it is set to whatever timerLength is. 
                                                                                //Last value is 0 since we don't have to wait for another knock.
unsigned int knocks[3] = {0, 0, 0};                                                //Stores all the knocks, not the actual code (see ^). Int because we store time in milliseconds/2 which goes over 255.
unsigned char sensitivity = 133;                                                 //Must be greater than 0 otherwise everything counts as knock, not sure what value this should be (requires testing).
unsigned char micInput;                                                         //Stores the value returned from adConvert(MIC).
unsigned char knockIndex;                                                       //Used in functions.
unsigned char i;                                                                         //Not unsigned because it needs to reach -1.
unsigned int time = 0;                                                          //This is an int since it is counting time between knocks.
unsigned int timerLength = 20;

bool isCounting = false;                                                        //Checks if we should be timing.
bool knockEnded = true;                                                         //When the microphone input goes above sensitivity, we only want it to record 1 knock, this will check if it has gone back down.

void interrupt checkInterrupt(void) {                                               //Function to check timer overflow (Assuming called automatically? Need confirmation.
                                                                                //Also need to check if it calls the interrupt while something else is running, such as checkKnocks()).
    if(TMR0IF == 1){                                                            //Timer overflowed.
        TMR0 = 100;                                                             //Reset value back to 100 (reasoning listed in main()).
        TMR0IF = 0;                                                             //Reset timer interrupt.
        if(isCounting){
            time++;                                                             //~2ms went by, increase the count.
        }
    }
}

unsigned char adConvert(unsigned char chan) {                                   //Function used to convert analogue input from the microphone to a digital value.
    ADON = 1;
    ADCON0 = (ADCON0 & 0b10000011);
    ADCON0 = (ADCON0 | chan);
    __delay_us(2);

    GO = 1;
    while (GO);

    ADON = 0;
    return (ADRESH);
}

void beep(unsigned char period, unsigned char cycles) {                         //Function to make the beep.
	unsigned char i;
	unsigned char t;
	for (i = cycles; i != 0; i --)                                              // number of beeper pin output flips.
	{
		BEEPER = !BEEPER;                                                       // flip beeper pin and
		for (t = period; t != 0; t --);                                         // wait for period to end.
	}
}

void unlock(unsigned char duration) {                                           //Function to unlock the lock. Duration is in seconds.
    LED1 = 1;                                                                   //Turn on the LED so if it is in a dark area the user can tell it is open.
    LOCK = 1;                                                                   //Unlock the door.
    for(i = duration; i != 0; i--){
        __delay_ms(125);                                                       //Delay for duration * 1000 (ms to seconds).
    }
    LOCK = 0;                                                                   //Re-lock the door.
    LED1 = 0;                                                                   //Turn off the light.
    __delay_ms(200);                                                            //Incase the sound from it locking is sensed as a knock.
}

bool isCorrect() {
    if(knockIndex + 1 != codeLength) {                                          //If the number of knocks isn't right, then the code can't be.
        return false;
    }
    knockIndex = 0;                                                             //Reusing the variable to store the number of correct knocks.
    for(i = 0; i != codeLength; i--){
        if(knocks[i] > code[i]*timerLength){                                            //If knock was longer, //Multiply by timerLength to scale back to milliseconds instead of quarter notes
            if(knocks[i]-timerLength/2 <= code[i]*timerLength){                                   //If it is within 200ms,
                knockIndex++;                                                   //Increase number of correct knocks. Again, reusing this variable as a counter.
            } else {
                return false;                                                   //Code can't be correct.
            }
        } else if(knocks[i] != code[i]){                                        //Otherwise, if knock was shorter but not equal (use != because it is faster runtime).
            if(knocks[i]+timerLength/2 >= code[i]*timerLength){                                   //If it is within 200ms,
                knockIndex++;                                                   //Increase number of correct knocks. Again, reusing this variable as a counter.
            } else {
                return false;                                                   //Code can't be correct.
            }
        } else {
            knockIndex++;                                                       //It should only reach here if (for some reason) the knock timing is exact to the millisecond.
        }
    }
    return knockIndex == codeLength;                                            //Returns true if it is correct, false if it isn't.
}

void playKnock(){
    for(i = 0; i != codeLength; i++){                                           //Loop for each element in the array
        beep(200,200);                                                          //Make the beep sound.
        for(unsigned char j = code[i]; j != 0; j--){                            //Delay for the set time
            __delay_ms(20);
        }
    }
}

void checkKnocks() { 
    __delay_ms(15);                                                             //Delay for a bit so that it doesn't pick up the knock echo (just records it once).
    for(i = codeLength; i != 0; i--){                                          //Loop through the length of the array.
        knocks[i-1] = 0;                                                          //Reset all read knocks to 0.
    }
    knockIndex = 0;                                                             //Reset the index of where to store the knock in knocks[].
    isCounting = true;                                                          //Start counting.
    time = 0;                                                                   //Reset time counter.
    while(time != 2000 && knockIndex + 1 != codeLength) {                       //Loop until we have the correct number of knocks or there has been 4 seconds without a knock (number could be changed in future).                                                                //Using a Do-While because we always want to check for the next knock (unless the knock code length is 1, which it isn't for now).
        micInput = adConvert(MIC);                                              //Get the microphone input and store it.
        if(micInput >= sensitivity){                                            //If it is loud enough.
            knocks[knockIndex] = time*2;                                        //Store the time between knocks.  *2 because we measure in chunks of 2ms, so this will convert to 1ms.
            knockIndex++;                                                       //Move the index to store the next knock.
            time = 0;                                                           //Reset the time counter.
            __delay_ms(15);                                                     //Delay for a bit so that it doesn't pick up the knock echo (just records it once).
        }/* else if(!knockEnded){                                               //If the knock sound has not been recorded to have gone below the threshold.
            if(micInput < sensitivity){                                         //Check if it has.
                knockEnded = true;                                              //If so, record that it has.
            }
        }*/
    }
    
    isCounting = false;                                                         //Stop counting.
    time = 0;                                                                   //Reset time counter.
    if(isCorrect()){                                                            //Check if knock code was correct.
        unlock(3);                                                              //If so, unlock door for 3 seconds (time can change, could use variable for this and let user set it).
        beep(200, 200);
    }
}

int main(void) {                                                                //Function that runs on startup.
    init();                                                                     //Initialize oscillator and I/O Ports.
    TMR0 = 100;                                                                 //The way our code works, this sets the time value to overflow every ~0.001997s, which is around 2ms. Important for timing notes.                                                                                
                                                                                //Formula for calculating delay: Delay = ((256-REG_val)*(Prescal*4))/Fosc   //Oscillator Frequency for 12F1840 is set in header file.
    __delay_us(200);                                                            
    unlock(1);                                                                  //Incase I forget the knock code. Also lets us know the circuit is on.
    ei();
    playKnock();                                                                //Plays the current knock pattern
    while (1) {
        micInput = adConvert(MIC);
        if(micInput >= sensitivity){
//            knockEnded = false;
            checkKnocks();
        }
        __delay_us(20);
    }
}