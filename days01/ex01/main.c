#include <avr/io.h>

#define LED_D2 (1 << PB1)
#define TIME_TARGET 7812

void setup()
{
    DDRB |= LED_D2; // led defi
    // ce qui nous donnera une fréquence de 4 MHz en sortie du prescaler (interne au µC ATmega328P, pour rappel)
    //régler le PRÉDIVISEUR du timer1 pour une division par 64. Ainsi, la fréquence qui cadencera le timer1 sera 64 fois plus lente que celle sortant du prescaler, qui pour rappel était de 4 MHz. 
    // Ainsi, la fréquence d’avancement du timer1 sera de 4 MHz divisé par 64, soit 62500 Hz. 
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    //Bit 7:6 – COM1A1:0: Compare Output Mode for Channel A
    TCCR1A |= (1 << COM1A0); // toggle le channel A ou se situe la led
    TCNT1 = 0; // on commencera par compter a cette valeur
    OCR1A = TIME_TARGET ;        // Valeur correspondant à 500ms


}


int main ()
{
    
    setup();
    while (1)
    {}
}
