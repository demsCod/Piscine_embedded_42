#include "uart_rgb.h"

void init_rgb()
{
    cli();
    CLKPR |= (1 << CLKPS3); // Prescaler  256

    TCCR0A |= (1 << WGM00) | (1 << WGM01);                 // Mode  Fast PWM R0
    TCCR2A |= (1 << WGM20) | (1 << WGM21);                 // Mode  Fast PWM R2

    TCCR0A |= (1 << COM0B1) | (1 << COM0A1);  // green and red 
    TCCR2A |= (1 << COM2B1);    // blue

    TCCR0B |= (1 << CS01) | (1 << CS00);   // Prescaler 64
    TCCR2B |= (1 << CS22) | (1 << CS20);  // Prescaler 64
    sei();
}


void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0A = g;
    OCR0B = r;
    OCR2B = b;
}
