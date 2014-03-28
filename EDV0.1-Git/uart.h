#ifndef __uart_h
#define __uart_h
#include "type.h"

void usart0_init(void);
void UartClearRxd(void);
void UartLpm(void);
void UartWriteChar(char c);
void UartWriteStr(char *s);
void UartWriteStrNum(char *s,char num);
void UartSendChar(char data);


#endif /* __uart_h */

/* end of uart.h */





