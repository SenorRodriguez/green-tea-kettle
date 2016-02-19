/*
 * onewire.h
 *
 * Created: 19.02.2016 17:03:26
 *  Author: Zbyszek
 */ 


#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include <avr/io.h>

//define connection
#define OW_PORT PORTB
#define OW_DDR	DDRB
#define OW_PIN	PINB
#define OW_DQ	PB3		//OneWire data line

uint8_t ow_reset();
void ow_writebit(uint8_t bit);
uint8_t ow_readbit(void);
void ow_writebyte(uint8_t data);
uint8_t ow_readbyte(void);


#endif /* ONEWIRE_H_ */