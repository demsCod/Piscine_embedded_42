/* Vous devez écrire un programme qui change l’état de la LED D1 lorsque l’on
appuie sur le bouton SW1.
• Vous devez utiliser les interruptions pour lire l’état du bouton. Il est interdit de
lire les registres PINx*/


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


// PB1 == CINT1/OC1A

void set_PWM()
{
    cli();
    sei();
}

volatile int is_press = 1;
ISR(PCINT2_vect)
{
    _delay_ms(50);
    is_press = !is_press;
}
int main()
{
    DDRB |= (1 << PB0); 
    

    set_PWM();
    while(1)
    {
      
    }
}
