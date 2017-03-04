#ifndef UART_h
	#define UART_h

#ifndef F_CPU								// if CPU frequency is not defined
	#define F_CPU 16000000UL
#endif

/************************************************************************************************************
 * 																											*
 * defines for UART communication																			*
 * 																											*
 ************************************************************************************************************/

// define Baud rate
#define BAUD 19200

// define RATIO (-> N inuser guide page 15-18)
#define RATIO ((double)F_CPU/BAUD)

// determine from baud rate and frequency the value for the UCBRx register
#define MYUCBRx ((int)RATIO)

// determine from UCBRx value the correction value for register UCBRSx
#define MYUCBRSx ((int)((RATIO - (int)RATIO) * 8.0 + 0.5)) // 0.5 for correct rounding

/************************************************************************************************************
 * 																											*
 * routines for USART initialization 																		*
 * 																											*
 ************************************************************************************************************/

void UART_Init();

void UartPutCmd0(unsigned char cmd[], unsigned char cmd_length);
void UartPutADC12Val(unsigned int value);

void UartPutShort(short value);
void UartPutChar(char character);
void UartPutStr(char* str, int length);

// Receiving not tested yet
void UartClearRX0();
unsigned char UartReadChar0();

#endif  //UART_h
