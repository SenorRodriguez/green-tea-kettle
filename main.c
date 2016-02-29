#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "onewire.h"
#include "ds18b20.h"
#include <stdio.h>

#define _DEBUG_ 0
#define MAX_DEVICES 4



int main (void) {
	uint64_t id = 0;
	uint64_t devTable[4];
	uint8_t deviceCount=0;
	uint8_t LD =0 ;		//lastDeviation,
	uart_tx_init ();	//UART initialization
	uart_print("\e[1;1H\e[2J");	//clear terminal window
	char buffer[50];
	do 
	{
		LD = ow_enumerate(&id, LD);
		if (LD==0xff) 
			break;
		else
			devTable[deviceCount++]=id;
	}
	while (LD);
	
	snprintf(buffer, 50, "Found: %d device(s)\n\r", deviceCount);
	uart_print(buffer);
	for (int i=0; i<deviceCount; i++)
	{
		uart_print("\n\r");
		snprintf(buffer, 50, "Device %d:\n\r", i);
		uart_print(buffer);
		snprintf(buffer, 50, "  Family: %#04x\n\r", (uint8_t)devTable[i] & 0xff);
		uart_print(buffer);
		uint64_t thisID = (devTable[i]<<8)>>16;//shaking off leftmost and rightmost 8 bits
		snprintf(buffer, 50, "      ID: %02x:%02x:%02x:%02x:%02x:%02x\n\r", (uint8_t)(thisID>>40),(uint8_t)(thisID>>32),(uint8_t)(thisID>>24),(uint8_t)(thisID>>16),(uint8_t)(thisID>>8),(uint8_t)thisID);
		uart_print(buffer);	
		snprintf(buffer, 50, "     CRC: %#04x\n\r", (uint8_t)(devTable[i] >> 56));
		uart_print(buffer);
	}
	uart_print("\n\r");
	while (1) {
		#if _DEBUG_ == 1
		uint8_t ow_status = ow_reset();
		if (ow_status){
			uart_print("OneWire: Read error or no devices found!!!\n\r");
		}
		else {
			uart_print("OneWire: OK\n\r");
		}
		#endif // _DEBUG_
		ds18b20_convert();
		//uart_print("\e[1;1H\e[2J");				//clear terminal window
		for(int i=0; i<deviceCount; i++)
		{
			int temp_raw = ds18b20_readtemp(devTable[i]);
			int temp_whole = temp_raw>>4;
			float temp_frac = (float)(temp_raw & 0x0f)/16;
			snprintf(buffer, 50, "Device %d: %d.%d \n\r", i+1, temp_whole, (uint8_t)(temp_frac*10));
			uart_print(buffer);
		}
		uart_print("\033[18;1H"); //go to row 18, col 1
		uart_print("\033[18;1H"); 
		uart_print("\033[18;1H");
		_delay_ms(500);
	}
	return 0;
}

