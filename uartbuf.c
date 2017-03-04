#include "uartbuf.h"
#include <stdlib.h>
#include <string.h>
#include "msp430g2553.h"

#define LISTEN_STATE 0
#define NUMBER_STATE 1
#define DONE_STATE 2

//volatile char rxBuffer[RXBUFFERSIZE];
//volatile unsigned int rxBufferFirstChar = 0;
//volatile unsigned int rxBufferLastChar = 0;
//volatile char rxBufferFull = 0;

char magicWord[6] = "FADER ";
char* mwPos = magicWord;
char rxedNumber[2] = "00";
char* rxPos = rxedNumber;
char state = LISTEN_STATE;
volatile char* faderLevelPtr = 0;

/************************************************************************************************************
 * 																											*
 * The recommended USCI initialization/re-configuration process is:											*
 * 																											*
 * 1) Set UCSWRST (BIS.B  #UCSWRST,&UCAxCTL1)																*
 * 2) Initialize all USCI registers with UCSWRST = 1 (including UCAxCTL1)									*
 * 3) Configure ports.																						*
 * 4) Clear UCSWRST via software (BIC.B  #UCSWRST,&UCAxCTL1)												*
 * 5) Enable interrupts (optional) via UCAxRXIE and/or UCAxTXIE												*
 * 																											*
 ************************************************************************************************************/
void UART_Init(volatile char* targetPos)
{
	faderLevelPtr = targetPos;
	
	// configure pins as UART
	P1SEL |= BIT1 | BIT2;
	P1SEL2 |= BIT1 | BIT2;

	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 104;                            // 1MHz 9600
	UCA0BR1 = 0;                              // 1MHz 9600
	UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1

	// activate UART
	UCA0CTL1 &= ~UCSWRST;

	// Delete initial interrupt flags
	IFG2 &= ~UCA0RXIFG;

	// enable RX interrupt
	IE2 |= UCA0RXIE;

	P2DIR |= BIT0;
	P2OUT |= BIT0;
}

void UartPutChar(char character){
	while (!(IFG2&UCA0TXIFG));
		UCA0TXBUF = character;
}

void UartPutStr(char* str, int length){
	int i = 0;
	for(i=0;i<length;i++){
		UartPutChar(str[i]);
		if(str[i] == '\0') break;
	}
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	P2OUT &= ~BIT0;
	switch (state){
	case LISTEN_STATE:	// waiting for magic word to complete
		if(*mwPos++ != UCA0RXBUF){
				mwPos = magicWord;
		}
		else if(mwPos - magicWord == 6){
			mwPos = magicWord;
			state = NUMBER_STATE;
		}
		break;
	case NUMBER_STATE:
		*rxPos++ = UCA0RXBUF;
		if(rxPos - rxedNumber == 2){
			state = DONE_STATE; 	// this may also take longer since we are done receiving
			if(faderLevelPtr != 0){
				*faderLevelPtr = (char)(atoi(rxedNumber));
			}
			rxPos = rxedNumber;
			state = LISTEN_STATE;
		}
	}
	P2OUT |= BIT0;
}

