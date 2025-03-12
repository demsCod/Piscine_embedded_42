
#include "uart_rgb.h"

void uart_init()
{
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    BIT_SET(UCSR0B, (1 << RXEN0));
    BIT_SET(UCSR0B, (1 << TXEN0));
    // BIT_SET(UCSR0B, RXCIE0); // enable interrupt
}

void uart_tx(unsigned char data)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void uart_printstr(const char *str)
{
    while(*str)
    {
        uart_tx(*str);
        _delay_ms(5);
        ++str;
    }
}

char uart_rx(void)
{
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}


// TCCR0A COM0A1 COM0A0 COM0B1 COM0B0 – – WGM01 WGM00
// TCCR0B FOC0A  FOC0B   – –   WGM02  CS02 CS01 CS00 

void setup_led()
{
    BIT_CLEAR(PORTD, LED_BLUE | LED_GREEN | LED_RED );  // Éteint toutes les LEDs
    BIT_SET(DDRD, LED_BLUE | LED_GREEN | LED_RED);      // Configure en sortie
}


int main()
{
    uart_init();
    init_rgb();
    setup_led();
    uart_printstr("UART Ready!\r\n");
    while(1)
    {
        read_input();
        _delay_ms(10);
    }
}