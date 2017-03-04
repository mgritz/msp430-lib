#ifndef UART_h
	#define UART_h

void UART_Init(volatile char* targetPos);

void UartPutChar(char character);
void UartPutStr(char* str, int length);

#endif  //UART_h
