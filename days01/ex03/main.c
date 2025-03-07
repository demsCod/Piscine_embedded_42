#include <avr/io.h>
#include <util/delay.h>

#define LED_D2 (1 << PB1)
#define TIME_TARGET 7812
#define BUTTON_1 (1 << PD2)
#define BUTTON_2 (1 << PD4)

static int index = 0;

float cycles[10] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
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
    OCR1A = TIME_TARGET * cycles[index];  // BOTTOM      
}

void handle_button()
{
    if (!(PIND & BUTTON_1))
    {
        _delay_ms(50); // wait
        if(!(PIND & BUTTON_1))
        {
            if (index < 9)
                ++index;
            OCR1A = TIME_TARGET * cycles[index];
        }
        while(!(PIND & BUTTON_1));
    }
    if (!(PIND & BUTTON_2))
    {
        _delay_ms(50); // wait
        if(!(PIND & BUTTON_2))
        {
            if (index > 1)
                --index;
            OCR1A = TIME_TARGET * cycles[index];
        }
        while(!(PIND & BUTTON_2));
    }
}


int main ()
{
    
    setup();
    while (1)
    {
        handle_button();
    }
}