#ifndef STDUART_H
#define STRUART_H
//---------------------------------------------------------------------------------------------------
#define BAUD		9600
//---------------------------------------------------------------------------------------------------
#define UART_ECHO	7
#define UART_READY	6
//---------------------------------------------------------------------------------------------------
#define Uart0SetIntRXC() SBIT(UCSR0B,RXCIE0);
#define Uart0ClrIntRXC() CBIT(UCSR0B,RXCIE0);

#define Uart0SetIntTXC() SBIT(UCSR0B,TXCIE0);
#define Uart0ClrIntTXC() CBIT(UCSR0B,TXCIE0);

#define Uart0SetIntUDR() SBIT(UCSR0B,UDRIE0);
#define Uart0ClrIntUDR() CBIT(UCSR0B,UDRIE0);
//---------------------------------------------------------------------------------------------------
void UartStdInit( void );
void UartInit( void );
void UartSetFlagRxEcho(uint8_t f);

uint8_t UartRxCheckReady(void);
char *UartRxData(void);
uint8_t UartRxCount(void);
void UartDataClean(void);

/*
USART0
PORTE PE1 TXD0
PORTE PE0 RXD0
*/
//---------------------------------------------------------------------------------------------------
#endif

