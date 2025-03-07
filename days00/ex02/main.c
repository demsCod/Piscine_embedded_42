#include <avr/io.h>
#include <util/delay.h>

//DDRx : Registre qui impose la direction des ports. ex: DDRD=0b11110000 ; les bits 0 à 3 du portA sont configurés en entrée et de 4 à 7 en sortie. 

//PORTx : Registre d'écriture des niveaux logiques sur le port x ex: PORTA=0b11110000 ; 
// Si le port est en sortie (voir DDRx), les bits 0 à 3 du portA passent à l’état bas et de 4 à 7 à l’état haut. 

//PINx : Registre de lecture des niveaux logiques sur le port x 

//3 ports PortB, PortC, PortD

int main()
{
    DDRB |= (1 << PB0); // set PB2 as an output which is LED;
    DDRD = DDRD & ~(1 << PD2); // SET PD2 as input
    PORTD |= (1 << PD2); //activate resistance
    while(1)
    {
        if (!(PIND &(1<<PD2))) // if PD2 (sw1) 
        {
            PORTB |= (1<<PB0);
            while(!(PIND &(1<<PD2)));
        }
        else
            PORTB &= ~(1<<PB0); //ACTIVE LED
    }
}
