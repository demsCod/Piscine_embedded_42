#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TIME_TARGET (F_CPU / 1024) // 15625 DONC 1sec
#define BIT_SET(reg, target) reg |= (1 << target)
#define BIT_RESET(reg) reg = 0
#define LED_BLUE PD3
#define LED_RED PD5
#define LED_GREEN PD6


void setup_timer()
{
    cli();
    //régler le PRÉDIVISEUR du timer1 pour une division par 64. Ainsi, la fréquence qui cadencera le timer1 sera 64 fois plus lente que celle sortant du prescaler, qui pour rappel était de 4 MHz. 
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10); //  divison par 1024 donc 16 / 1024 = 15625 F_CPU
    BIT_SET(TIMSK1, OCIE1A);
    OCR1A = TIME_TARGET ;
    TCNT1 = 0;
    sei();
}

void setup_led()
{
    BIT_RESET(PORTD);
    BIT_SET(DDRD, LED_BLUE);
    BIT_SET(DDRD, LED_GREEN);
    BIT_SET(DDRD, LED_RED);
}

ISR (TIMER1_COMPA_vect)
{
    static int color_table[3] = {LED_RED, LED_GREEN, LED_BLUE};
    static int i = 0;

    BIT_RESET(PORTD);
    BIT_SET(PORTD, color_table[i % 3]);
    ++i;
}


int main ()
{
    setup_timer();
    setup_led();
    while(1)
    {}
}