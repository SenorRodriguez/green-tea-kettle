#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "onewire.h"
#include "ds18b20.h"
#include <stdio.h>

#define _DEBUG_ 0

int main (void) {
	int64_t id = 0;
	uint8_t LD =0 ;			//lastDeviation,
	uart_tx_init ();	//UART initialization
	//_delay_ms(250);
	uart_print("\e[1;1H\e[2J");
	char buffer[50];
	do 
	{
		LD = ow_enumerate(&id, LD);
		if (LD==0xff) 
		{
			uart_print("\n\rFailed\n\r");
			break;
		}
		else
		{
			uart_print("\n\r");
			for(int i=0; i<64; i++)
			{
				if (id & ((int64_t)1<<(63-i)))
					uart_print("1");
				else
					uart_print("0");
			}
		}
		snprintf(buffer, 50, "\n\rLD = %d\n\r", LD);
		uart_print(buffer);
	}
	while (LD);
	 
	uart_print("\n\r");
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
		//uart_print("\e[1;1H\e[2J");				//clear terminal window
		uart_print("Current temperature: ");
		uart_print(s_whole_degrees);
		uart_print("\r");
		/*
		uint8_t low = temp_raw & 0xff;
		uint8_t high = (uint8_t)(temp_raw >> 8);
		uart_send_byte(high);
		uart_send_byte(low);
		uart_send_byte(0xff);
		*/
		_delay_ms(500);
	}
	return 0;
}

