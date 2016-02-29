/*
 * onewire.c
 *
 * Created: 19.02.2016 17:04:21
 *  Author: Zbyszek
 * Based on Atmel's AVR318 document, http://www.atmel.com/images/doc2579.pdf
 * My main goal was to create library independent of any other non-standard libraries (uart, etc.)
 */ 
#include "onewire.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//reset One wire communication
uint8_t ow_reset() {
	uint8_t i;

	//to reset, hold the line low 
	OW_PORT &= ~ (1<<OW_DQ);		//set register to low
	OW_DDR |= (1<<OW_DQ);			//make it happen, by setting DQ as output
	_delay_us(DELAY_H);				

	//release the line and wait for I us
	OW_DDR &= ~(1<<OW_DQ);			//set DQ as high impedance input, thus release line (it goes back to HI by pull-up resistor)
	_delay_us(DELAY_I);				//wait

	//get DQ state and wait J us
	i = (OW_PIN & (1<<OW_DQ));
	_delay_us(DELAY_J);

	// 0=at least one device present, 1=no devices detected
	return i;
}

void ow_writebit(uint8_t bit){
	//put line low for a while, by doing so we say to slaves we want to communicate to them.
	OW_PORT &= ~ (1<<OW_DQ);	//set a bit in register to O
	OW_DDR |= (1<<OW_DQ);		//set DQ as output
	_delay_us(DELAY_A);			//delay A

	//if we want to write 1, release the line 
	if(bit)
	OW_DDR &= ~(1<<OW_DQ);		//release by setting DQ in input mode

	//if we want to write 0, do nothing (line is being kept LO) and just wait B us, then release the line
	_delay_us(DELAY_B);
	OW_DDR &= ~(1<<OW_DQ);		//release line
}

uint8_t ow_readbit(void){
	uint8_t bit=0;

	//pull line low for A us
	OW_PORT &= ~ (1<<OW_DQ);		//same procedure as in ow_writebit
	OW_DDR |= (1<<OW_DQ);
	_delay_us(DELAY_A);

	//release line and wait for E us
	OW_DDR &= ~(1<<OW_DQ);			//set as input
	_delay_us(DELAY_E);					//14us, just in case

	//read the value
	if(OW_PIN & (1<<OW_DQ))
	bit = 1;

	//wait another 45us to fill the 60us read window
	_delay_us(DELAY_F);
	return bit;
}
void ow_writebyte(uint8_t data){
	uint8_t i=8;
	while(i--){					//repeat 8 times
		ow_writebit(data & 1);	//take LSB and transmit
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


//enumerate up to MAX_DEVICES devices on 1-Wire bus 
uint8_t ow_enumerate(volatile uint64_t* bitPattern, uint8_t lastDeviation){
	//init variables
	uint8_t newDeviation = 0;
	uint8_t bitIndex = 0;
    uint8_t bit1,bit2;
	cli();
	ow_reset();
	ow_writebyte(OW_CMD_SEARCHROM);
	while (bitIndex<64)
	{
		bit1 = ow_readbit();
		bit2 = ow_readbit();
		if (bit1 & bit2) return ERR_ROM_SEARCH_FAILED;	//if both bits are 1, return error
		
		if (bit1 | bit2){
			//if one of two bits is 1 it means we have at least 2 devices, and their addresses are the same up to index [bitIndex]
			if (bit1)
			*bitPattern |= (uint64_t)1<<bitIndex;		//store 1
			else
			*bitPattern &= ~((uint64_t)1<<bitIndex);	//store 0
		}
		else{
			//if both bits are 0 it means we have at least 2 devices, and their addresses differ at index [bitIndex]
			if (bitIndex==lastDeviation){
				*bitPattern |= (uint64_t)1 << bitIndex;					//set bitPattert[bitIndex] to 1
			}
			else{ 
				if(bitIndex>lastDeviation) {
					*bitPattern &= ~((uint64_t)1 << bitIndex);			//set bitPattert[bitIndex] to 0
					newDeviation = bitIndex;
				}
				else{
					if (!(*bitPattern & ((uint64_t)1 << bitIndex))){	//if bitPattern[bitIndex]==0
						newDeviation = bitIndex; 
					}
				}
			}
		}
		if (*bitPattern & (uint64_t)1 << bitIndex)						//test bitPattern[bitIndex]
			ow_writebit(1); 
		else	
			ow_writebit(0);
		bitIndex++;
	}
	sei();
	return newDeviation;
}

uint8_t ow_rom_decode(uint64_t* id, uint8_t* familyCode, uint32_t* serialNumber, uint8_t CRC)
{
	return 1;
}