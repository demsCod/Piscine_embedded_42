#include <avr/io.h>
#include <util/delay.h>

//DDRx : Registre qui impose la direction des ports. ex: DDRD=0b11110000 ; les bits 0 à 3 du portA sont configurés en entrée et de 4 à 7 en sortie. 

//PORTx : Registre d'écriture des niveaux logiques sur le port x ex: PORTA=0b11110000 ; 
// Si le port est en sortie (voir DDRx), les bits 0 à 3 du portA passent à l’état bas et de 4 à 7 à l’état haut. 

//PINx : Registre de lecture des niveaux logiques sur le port x 

//3 ports PortB, PortC, PortD

// 110001
// 000110

// 110111

// 0001
// 0100

//PIND = 1111
// si pd2 n' est pas a son etat initial pind == 1101
//


int main()
{
    DDRB = DDRB | (1 << PB0); // set PB2 as an output which is LED;
    PORTB |= (1<<PB0); //active led
    PORTB = !(PORTB);
    while(1)
    {
        if (!(PIND &(1<<PD2))) // if PD2 (sw1) is pressed
        {
            _delay_ms(50); // wait
            if(!(PIND &(1<<PD2)))
                PORTB = !(PORTB); //inverse PORTB
            while(!(PIND &(1<<PD2)));
        }
    }

}
