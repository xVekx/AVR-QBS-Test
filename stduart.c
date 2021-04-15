#include "define.h"
//---------------------------------------------------------
#define BUFF_SIZE_RX 64 //размер буфера приема
#define BUFF_SIZE_TX 64 //размер буфера передачи
//---------------------------------------------------------
static char buff_rx [BUFF_SIZE_RX];
//static char buff_tx [BUFF_SIZE_TX];
//---------------------------------------------------------
static volatile uint8_t count_rx = 0;
static volatile uint8_t count_tx = 0;
//---------------------------------------------------------
static volatile uint8_t flag_rx = 0;
static volatile uint8_t flag_tx = 0;
//---------------------------------------------------------
void UartSetFlagRxEcho(uint8_t f)
{
	f ? SBIT(flag_rx,UART_ECHO) : CBIT(flag_rx,UART_ECHO);
}
//---------------------------------------------------------
#if defined(__AVR_ATmega2560__)
 ISR(USART0_RX_vect)
#elif defined (__AVR_ATmega328P__)
 ISR(USART_RX_vect)
#elif defined (__AVR_ATmega644PA__)
 ISR(USART0_RX_vect)
#elif defined (__AVR_ATmega644P__)
 ISR(USART0_RX_vect)
#endif
{
	char c = UDR0;
	if(TBIT1(flag_rx,UART_ECHO))
		printf("%c",c);
	if(TBIT0(flag_rx,UART_READY)) {
		if(c == '\n' || c == '\r') {
			buff_rx[count_rx] = '\0';
			SBIT(flag_rx,UART_READY);
		} else {
			buff_rx[count_rx] = c;
			if(count_rx < BUFF_SIZE_RX)
				count_rx++;
			else
				SBIT(flag_rx,UART_READY);
		}
	}
}
//---------------------------------------------------------
uint8_t UartRxCheckReady(void)
{
	return TBIT1(flag_rx,UART_READY);
}
//---------------------------------------------------------
char *UartRxData(void)
{
	return buff_rx;
}
//---------------------------------------------------------
uint8_t UartRxCount(void)
{
	return count_rx;
}
//---------------------------------------------------------
void UsartFlush( void )
{
	while (TBIT1(UCSR0A,RXC0))UDR0;
}
//---------------------------------------------------------
void UartDataClean(void)
{
	count_rx = 0;
	UsartFlush();
	CBIT(flag_rx,UART_READY);
}
//---------------------------------------------------------
int UartPutChar(char c, FILE *stream)
{
	if (c == '\n')
		UartPutChar('\r', stream);
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}
//---------------------------------------------------------
int UartGetChar(FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	return UDR0;
}
//---------------------------------------------------------
FILE uart_io = FDEV_SETUP_STREAM(UartPutChar, UartGetChar, _FDEV_SETUP_RW);
//---------------------------------------------------------
void UartInit(void)
{
	uint16_t ubrr = F_CPU/16/BAUD-1;
	/* Set baud rate */
	UBRR0H = (uint8_t)(ubrr>>8);
	UBRR0L = (uint8_t) ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = BIT(RXEN0) | BIT(TXEN0);

	//rUCSRC = (1<<bURSEL)|(1<<bUSBS)|(3<<bUCSZ0);

	// avr328 UCSR0C = BIT(USBS0) | (3<<UCSZ00) ;
	//UCSR0C |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	UCSR0C = BIT(USBS0) | (3<<UCSZ00) ;
	//UCSR0C |= (0<<USBS0);
}
//---------------------------------------------------------
void UartStdInit(void)
{
	stdout = &uart_io;
	stdin = &uart_io;
}
//---------------------------------------------------------
