/*
 * ds18b20.c
 *
 * Created: 18.02.2016 21:36:23
 *  Author: Zbyszek
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ds18b20.h"
#include "onewire.h"


//read temperature
int ds18b20_readtemp() {
	uint8_t temp_lo, temp_hi;
	int temp;

	#if DS18B20_INT_DISABLE == 1
	cli();
	#endif

	ow_reset();								//reset line
	ow_writebyte(DS18B20_CMD_SKIPROM);		//We don't need ROM info, so lets skip it
	ow_writebyte(DS18B20_CMD_CONVERTTEMP);	//Start conversion. Data will be available in scratchpad in next read cycle.

	while(!ow_readbit()); //Wait for conversion to complete. Until then DQ is being kept LOW
	//Conversion complete, lets read some data.
	ow_reset();								//reset line
	ow_writebyte(DS18B20_CMD_SKIPROM);		//skip ROM
	ow_writebyte(DS18B20_CMD_RSCRATCHPAD);	//read temperature from scratchpad

	//temperature is kept in two subsequent bytes.
	temp_lo = ow_readbyte();				//read lower half
	temp_hi = ow_readbyte();				//read higher half

	#if DS18B20_INT_DISABLE == 1
	sei();
	#endif
	//combine two bytes tgether
	temp = ( temp_hi << 8 ) + temp_lo;
	return temp;
}

