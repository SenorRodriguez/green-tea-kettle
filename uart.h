/*
 * uart.h
 *
 * Created: 18.02.2016 21:33:23
 *  Author: Zbyszek
 */ 


#ifndef UART_H_
#define UART_H_


#define SOFT_TX_PIN (1<<PB1) // PB1 ????? ???????? ??? TXD
#define SOFT_TX_PORT PORTB
#define SOFT_TX_DDR DDRB

void uart_tx_init ();
void num_to_str(unsigned int value, unsigned char nDigit);
void uart_print(char *str);
void uart_send_byte (unsigned char data);


#endif /* UART_H_ */