#include <avr/io.h>

#define LED_D2 (1 << PB1)
#define TIME_TARGET 7812

void setup()
{
    DDRB |= LED_D2; // led defini en sortie
    //Bit 6, 3 - WGM00, WGM01: Waveform Generation Mode
    TCCR1A |= (1 << COM1A1); // 1  0  Clear OC1A/OC1B on compare match, set OC1A/OC1B at BOTTOM (non-inverting mode)

    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13); // 1 1 1 0 Fast PWM ICR1 BOTTOM TOP


    TCCR1B |= (1 << CS12) | (1 << CS10);
    //Bit 7:6 – COM1A1:0: Compare Output Mode for Channel A
    TCNT1 = 0; // on commencera par compter a cette valeur
    ICR1 = TIME_TARGET ; // TOP
    OCR1A = TIME_TARGET * 0.10 ;  // BOTTOM      
}

int main ()
{
    
    setup();
    while (1)
    {}
}