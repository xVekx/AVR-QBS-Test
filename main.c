#include "define.h"
#include <avr/wdt.h>


///avrdude: safemode: Fuses OK (E:FF, H:DF, L:FF)

int main(void)
{
	wdt_enable(WDTO_8S);

	SBIT(DDRB,PB5);

	UartInit();
	UartStdInit();
	Uart0SetIntRXC();
	UartSetFlagRxEcho(!0);

	sei();

	//printf()

	//struct sMenu menu_tmp;
	//GetStruct_P(sMenu,&menu_tmp,m);

	printf("\n\nReset DEV\n");


	while (1) {
		wdt_reset();

		/*SBIT(PORTB,PB5);
		_delay_ms(1000);
		CBIT(PORTB,PB5);
		_delay_ms(1000);
		printf("Test\n");*/

		if(UartRxCheckReady()) {
			Uart0ClrIntRXC();
			char *data = UartRxData();
			uint8_t count = UartRxCount();

			printf("\n");
			PrintHexString(data,count);
			CommandComPROGMEM(&CommandComTest,data,count+1);

			//vsprintf(data);

			/*if(GuiCommand(data,count+1))
				GuiDraw();*/

			UartDataClean();
			Uart0SetIntRXC();
		}

	}
}
//---------------------------------------------------------------------------------------------------
