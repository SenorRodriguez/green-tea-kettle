/*
 * onewire.c
 *
 * Created: 19.02.2016 17:04:21
 *  Author: Zbyszek
 */ 
#include "onewire.h"
#include <avr/io.h>
#include <util/delay.h>

//reset One wire communication
uint8_t ow_reset() {
	uint8_t i;

	//to reset, hold the line low for 480 to 640us
	OW_PORT &= ~ (1<<OW_DQ);	//set register to low
	OW_DDR |= (1<<OW_DQ);		//make it happen, by setting DQ as output
	_delay_us(500);				//500us, just in case		

	//release the line and wait for 15 to 60us
	OW_DDR &= ~(1<<OW_DQ);		//set DQ as high impedance input, thus release line
	_delay_us(60);				//wait

	//get DQ state and wait 420us
	i = (OW_PIN & (1<<OW_DQ));
	_delay_us(420);

	// 0=at least one device present, 1=no devices detected
	return i;
}

void ow_writebit(uint8_t bit){
	//put line low for a while, by doing so we say to slaves we want to communicate to them.
	OW_PORT &= ~ (1<<OW_DQ);	//set a bit in register to O
	OW_DDR |= (1<<OW_DQ);		//set DQ as output
	_delay_us(1);

	//if we want to write 1, release the line (it goes back to HI by pull-up resistor)
	if(bit)
	OW_DDR &= ~(1<<OW_DQ);		//release by setting DQ in input mode

	//if we want to write 0, do nothing (line is being kept LO) and just wait 60us, then release the line
	_delay_us(60);
	OW_DDR &= ~(1<<OW_DQ);		//release line
}

uint8_t ow_readbit(void){
	uint8_t bit=0;

	//pull line low for 1us
	OW_PORT &= ~ (1<<OW_DQ);		//same procedure as in ow_writebit
	OW_DDR |= (1<<OW_DQ);
	_delay_us(1);

	//release line and wait for 15us max
	OW_DDR &= ~(1<<OW_DQ);			//set as input
	_delay_us(14);					//14us, just in case

	//read the value
	if(OW_PIN & (1<<OW_DQ))
	bit = 1;

	//wait another 45us to fill the 60us read window
	_delay_us(45);
	return bit;
}
void ow_writebyte(uint8_t data){
	uint8_t i=8;
	while(i--){					//repeat 8 times
		ow_writebit(data & 1);	//take LSB ant transmit
		data >>= 1;				//shift right
	}
}
uint8_t ow_readbyte(void){
	uint8_t i=8, byte=0;
	//bytes are sent LSB first. Read a bit, put in buffer in the MSB position, shift buffer one bit right. Repeat 8 times
	while(i--){
		byte >>= 1;
		byte |= (ow_readbit() << 7);
	}
	return byte;
}