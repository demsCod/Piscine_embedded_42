#ifndef UART_H
#define UART_H

#define BAUD 115200
#define MYUBRR (F_CPU/ (16 * BAUD))
#define BIT_SET(reg, target) reg |= (1 << target)
#define BIT_RESET(reg) reg = 0
#define LEDS 23

#include <util/delay.h>
#include <avr/interrupt.h>

void uart_init()
{
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    /*ACTIVER LES TRANSMISSION*/
    /*The USART transmitter is enabled by setting 
    the transmit enable (TXEN) bit in the UCSRnB register. */
    /*Enable receiver and transmitter */
    BIT_SET(UCSR0B, RXEN0);
    BIT_SET(UCSR0B, TXEN0);
    BIT_SET(UCSR0B, RXCIE0); // enable interupt
}

void uart_tx(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)))
        ;
    UDR0 = data;
}

void uart_printstr(const char* str)
{
    while(*str)
    {
        uart_tx(*str);
        _delay_ms(5);
        ++str;
    }
}

#endif