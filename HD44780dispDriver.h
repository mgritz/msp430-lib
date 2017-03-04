
//
// MSP430 LCD Code
//

#include  <msp430.h>
#include  <string.h>

#define		LCM_CTRL_DIR	P1DIR
#define     LCM_CTRL_OUT    P1OUT
#define     LCM_DATA_DIR	P2DIR
#define		LCM_DATA_OUT	P2OUT

//
// Define symbolic LCM - MCU pin mappings
//
//
#define     LCM_PIN_RS            BIT6          // P1.6
#define     LCM_PIN_EN            BIT7          // P1.7

#define     LCM_PIN_D4            BIT0          // P2.0
#define     LCM_PIN_D5            BIT1          // P2.1
#define     LCM_PIN_D6            BIT2          // P2.2
#define     LCM_PIN_D7            BIT3          // P2.3


#define     LCM_DATA_PIN_MASK  ((LCM_PIN_D7 | LCM_PIN_D6 | LCM_PIN_D5 | LCM_PIN_D4))
#define		LCM_CTRL_PIN_MASK  ((LCM_PIN_RS | LCM_PIN_EN))

#define     FALSE                 0
#define     TRUE                  1

//
// Routine Desc:
//
// This is the function that must be called
// whenever the LCM needs to be told to
// scan it's data bus.
//
// Parameters:
//
//     void.
//
// Return
//
//     void.
//
void PulseLcm()
{
//	__bic_SR_register(GIE);
    //
    // pull EN bit low
    //
    LCM_CTRL_OUT &= ~LCM_PIN_EN;
    __delay_cycles(4000);

    //
    // pull EN bit high
    //
    LCM_CTRL_OUT |= LCM_PIN_EN;
    __delay_cycles(4000);

    //
    // pull EN bit low again
    //
    LCM_CTRL_OUT &= (~LCM_PIN_EN);
    __delay_cycles(4000);
//    __bis_SR_register(GIE);
}



//
// Routine Desc:
//
// Send a byte on the data bus in the 4 bit mode
// This requires sending the data in two chunks.
// The high nibble first and then the low nible
//
// Parameters:
//
//    ByteToSend - the single byte to send
//
//    IsData - set to TRUE if the byte is character data
//                  FALSE if its a command
//
// Return
//
//     void.
//
void SendByte(char ByteToSend, int IsData)
{
	char highnibble = (ByteToSend & 0xF0);
	char lownibble = (ByteToSend & 0x0F);
    //
    // clear out all pins
    //
	LCM_CTRL_OUT &= (~LCM_CTRL_PIN_MASK);
	LCM_DATA_OUT &= (~LCM_DATA_PIN_MASK);
    //
    // set High Nibble (HN) -
    // We can set the DB7 - DB4 by setting P2.0 - P2.3
    //
    LCM_DATA_OUT |= (highnibble >> 4);

    if (IsData == TRUE)
    {
        LCM_CTRL_OUT |= LCM_PIN_RS;
    }
    else
    {
    	LCM_CTRL_OUT &= ~LCM_PIN_RS;
    }

    //
    // we've set up the input voltages to the LCM.
    // Now tell it to read them.
    //
    PulseLcm();
    //
    // set Low Nibble (LN) -
    // We can set the DB7 - DB4 by setting P2.0 - P2.3
    //
    LCM_DATA_OUT &= (~LCM_DATA_PIN_MASK);
    LCM_DATA_OUT |= lownibble;

    if (IsData == TRUE)
        {
            LCM_CTRL_OUT |= LCM_PIN_RS;
        }
        else
        {
        	LCM_CTRL_OUT &= ~LCM_PIN_RS;
        }

    //
    // we've set up the input voltages to the LCM.
    // Now tell it to read them.
    //
    PulseLcm();
}


//
// Routine Desc:
//
// Set the position of the cursor on the screen
//
// Parameters:
//
//     Row - zero based row number
//
//     Col - zero based col number
//
// Return
//
//     void.
//
void LcmSetCursorPosition(char Row, char Col)
{
    char address;

    //
    // construct address from (Row, Col) pair
    //
    if (Row == 0)
    {
        address = 0;
    }
    else
    {
        address = 0x40;
    }

    address |= Col;

    SendByte(0x80 | address, FALSE);
}


//
// Routine Desc:
//
// Clear the screen data and return the
// cursor to home position
//
// Parameters:
//
//    void.
//
// Return
//
//     void.
//
void ClearLcmScreen()
{
    //
    // Clear display, return home
    //
    SendByte(0x01, FALSE);
    SendByte(0x02, FALSE);
}


//
// Routine Desc:
//
// Initialize the LCM after power-up.
//
// Note: This routine must not be called twice on the
//           LCM. This is not so uncommon when the power
//           for the MCU and LCM are separate.
//
// Parameters:
//
//    void.
//
// Return
//
//     void.
//
void InitializeLcm(void)
{
    //
    // set the MSP pin configurations
    // and bring them to low
    //
    LCM_DATA_DIR |= LCM_DATA_PIN_MASK;
    LCM_DATA_OUT &= ~(LCM_DATA_PIN_MASK);

    LCM_CTRL_DIR |= LCM_CTRL_PIN_MASK;
    LCM_CTRL_OUT &= ~(LCM_CTRL_PIN_MASK);


    //
    // wait for the LCM to warm up and reach
    // active regions. Remember MSPs can power
    // up much faster than the LCM.
    //
    __delay_cycles(100000);


    //
    // initialize the LCM module
    //
    // 1. Set 4-bit input
    //
    LCM_CTRL_OUT &= ~LCM_PIN_RS;
    LCM_CTRL_OUT &= ~LCM_PIN_EN;

    LCM_DATA_OUT = 0x02;
    PulseLcm();

    //
    // set 4-bit input - second time.
    // (as reqd by the spec.)
    //
    SendByte((char)0x28, FALSE);

    //
    // 2. Display on, cursor on, blink cursor
    //
    SendByte((char)0x0E, FALSE);

    //
    // 3. Cursor move auto-increment
    //
    SendByte((char)0x06, FALSE);
}


//
// Routine Desc
//
// Print a string of characters to the screen
//
// Parameters:
//
//    Text - null terminated string of chars
//
// Returns
//
//     void.
//
void PrintStr(char *Text)
{
    char *c;

    c = Text;

    while ((c != 0) && (*c != 0))
    {
        SendByte(*c, TRUE);
        c++;
    }
}
