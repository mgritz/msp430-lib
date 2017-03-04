#include "uart.h"
#include <stdio.h>

#include  "msp430f2419.h"

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

 
 /*
 UART 0 - MSP430 <-> FT2232
 */
 

//Routinen f�r USART Initialisierung
void UART_Init()
{
	P3SEL |= BIT4 | BIT5;     // P3.4 und P3.5 als USART0 TXD/RXD,

	// USCI clock source select -> choose ACLK
	UCA0CTL1 |= UCSSEL0;                 // UART 0
	
	// set Baud rate for module 0
	UCA0BR0 = (unsigned char)(MYUCBRx);
	UCA0BR1 = (unsigned char)(MYUCBRx>>8);

	// set another bloody register for module 0 (baudrate correction)
	//UCA0MCTL = (unsigned char)(MYUCBRSx<<1); // should work, but the following seems better, here: 3, table in user guide: 2
	UCA0MCTL = UCBRS_2;
	
	// set frame format:  8data, 2stop bit, no parity
	UCA0CTL0 = UCSPB;
	
	// activate UART
	UCA0CTL1 &= ~UCSWRST;

	// Delete initial interrupt flags    
	IFG2 &= ~UCA0RXIFG;
	UC1IFG &= ~UCA1RXIFG;
}


void UartPutCmd0(unsigned char cmd[], unsigned char cmd_length) {
	unsigned char i;
	for (i = 0; i != cmd_length; ++i) {
		while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?	
		UCA0TXBUF = cmd[i];
	}
}

void UartPutADC12Val(unsigned int value) {
	/* converts an int value to a 4 character string.
	 * string is terminated by a line feed command ( '\n' ASCII Code 10 (dec) / 0x0A (hex) )
	 * */
	volatile unsigned int i;
	volatile unsigned zehner1 = 1;
	volatile unsigned zehner2 = 10;
	int ziffer = 0;
	unsigned char str[4];	// ADC-Werte sind 4 Ziffern lang
	for(i=0; i<4; i++)
	{
		
		ziffer = (value%zehner2) / zehner1;
		
		zehner1 = zehner2;
		zehner2 *= 10;
		str[3-i] = '0'+ ziffer;
	}
	
	for(i=0; i<4; i++)
	{
		while (!(IFG2&UCA0TXIFG));	// Warte bis gesendet werden kann
		UCA0TXBUF = str[i];
	}
	while (!(IFG2&UCA0TXIFG));
	UCA0TXBUF = '\n';
}

void UartPutShort(short value){
	char lsby = value;
	char msby = value >> 8;
	while (!(IFG2&UCA0TXIFG));
	UCA0TXBUF = lsby;
	while (!(IFG2&UCA0TXIFG));
	UCA0TXBUF = msby;
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

void UartClearRX0() {
	IFG2 &= ~UCA0RXIFG;
}

unsigned char UartReadChar0() {
	while (!(IFG2&UCA0RXIFG));  		// USCI_A0 RX buffer ready?	
	return UCA0RXBUF;
}
