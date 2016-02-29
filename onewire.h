/*
 * onewire.h
 *
 * Created: 19.02.2016 17:03:26
 *  Author: Zbyszek
  * Based on Atmel's AVR318 document, http://www.atmel.com/images/doc2579.pdf
  * My main goal was to create library independent of any other non-standard libraries (uart, etc.)
 */ 


#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include <avr/io.h>

//define connection
#define OW_PORT PORTB
#define OW_DDR	DDRB
#define OW_PIN	PINB
#define OW_DQ	PB3		//OneWire data line

//delay definitions

#define DELAY_A 6
#define DELAY_B 64
#define DELAY_C 60
#define DELAY_D 10
#define DELAY_E 9
#define DELAY_F 55
#define DELAY_G 0
#define DELAY_H 480
#define DELAY_I 70
#define DELAY_J 410

#define OW_CMD_SEARCHROM		0xf0
#define OW_CMD_MATCH_ROM		0x55
#define OW_CMD_SKIP_ROM			0xcc


#define ERR_ROM_SEARCH_FAILED	0xff



uint8_t ow_reset();
void ow_writebit(uint8_t bit);
uint8_t ow_readbit(void);
void ow_writebyte(uint8_t data);
uint8_t ow_readbyte(void);
uint8_t ow_enumerate(volatile uint64_t* bitPattern, uint8_t lastDeviation);


#endif /* ONEWIRE_H_ */