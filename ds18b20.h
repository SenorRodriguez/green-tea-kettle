/*
 * ds18b20.h
 *
 * Created: 18.02.2016 21:36:23
 *  Author: Zbyszek
 */ 

#ifndef DS18B20_H_
#define DS18B20_H_

//commands definitions
#define DS18B20_CMD_CONVERTTEMP		0x44
#define DS18B20_CMD_RSCRATCHPAD		0xbe
#define DS18B20_CMD_WSCRATCHPAD		0x4e
#define DS18B20_CMD_CPYSCRATCHPAD	0x48
#define DS18B20_CMD_RECEEPROM		0xb8
#define DS18B20_CMD_RPWRSUPPLY		0xb4
#define DS18B20_CMD_SEARCHROM		0xf0
#define DS18B20_CMD_READROM			0x33
#define DS18B20_CMD_MATCHROM		0x55
#define DS18B20_CMD_SKIPROM			0xcc
#define DS18B20_CMD_ALARMSEARCH		0xec

//Reading and writing data from ds chip is time-based, so we may want disable interrupts while doing so.
#define DS18B20_INT_DISABLE 1

//functions
int ds18b20_readtemp();

#endif
