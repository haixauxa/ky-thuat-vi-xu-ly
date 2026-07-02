#ifndef USART_H
#define USART_H

#include "stm32f10x.h"                  // Device header

typedef struct  {
	void (*init)(uint32_t baudrate);
	void (*send_char)(char);
	void (*send_string)(char *);
} Uart_Struct_t __attribute__((__aligned__(32)));

extern Uart_Struct_t uart1;
extern Uart_Struct_t uart2;
extern Uart_Struct_t uart3;

void Ip_Uart_Init(USART_TypeDef *USARTx, uint32_t baudrate);
void Ip_Uart_Send_Char(USART_TypeDef *USARTx, char c);
void Ip_Uart_Send_String(USART_TypeDef *USARTx, char *s);

#endif /* USART_H */
