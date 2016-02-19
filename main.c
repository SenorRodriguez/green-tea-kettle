#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "ds18b20.h"
#include <stdio.h>

#define _DEBUG_ 0

int main (void) {
	uart_tx_init (); //UART initialization
	while (1) {
	#if _DEBUG_ == 1
		uint8_t ow_status = ds18b20_reset();
		if (ow_status){
			uart_print("OneWire: Error.\n\r");
		}
		else {
			uart_print("OneWire: OK\n\r");
		}
	#endif // _DEBUG_
		int temp_raw = ds18b20_readtemp();
		//double temp = (double)temp_raw/16;	//because 4 less significant bits mean fractions of degree
		char s_whole_degrees[10];
		snprintf(s_whole_degrees, 10, "%d", (int)temp_raw>>4);
		uart_print("\e[1;1H\e[2J");				//clear terminal window
		uart_print("Current temperature: ");
		uart_print(s_whole_degrees);
		uart_print("\n\r");
		_delay_ms(500);
	}
	return 0;
}

