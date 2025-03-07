#include <avr/io.h>
#include <util/delay.h>

//DDRx : Registre qui impose la direction des ports. ex: DDRD=0b11110000 ; les bits 0 à 3 du portA sont configurés en entrée et de 4 à 7 en sortie. 

//PORTx : Registre d'écriture des niveaux logiques sur le port x ex: PORTA=0b11110000 ; 
// Si le port est en sortie (voir DDRx), les bits 0 à 3 du portA passent à l’état bas et de 4 à 7 à l’état haut. 

//PINx : Registre de lecture des niveaux logiques sur le port x 

//3 ports PortB, PortC, PortD

int main()
{
    DDRB |= 23; // set all PB2 as an output which is LED;
    //PORTB |= (1<<PB0); //active led

    while(1)
    {
        if (!(PIND &(1<<PD2))) // if PD2 (sw1) is pressed
        {
            _delay_ms(50); // wait
            if (!(PIND &(1<<PD2)) && PORTB < 23)
            {
                PORTB +=1; //INCREMENT PORTB
                if (PORTB == 8)
                    PORTB = 16;
            }
            while(!(PIND &(1<<PD2)));
        }
        if (!(PIND &(1<<PD4))) // if PD2 (sw1) is pressed
        {
            _delay_ms(50); // wait
            if (!(PIND &(1<<PD4)) && PORTB > 0)
            {
                if (PORTB == 8)
                    PORTB = 16;
                PORTB -=1; //DECREMENT PORTB
            }
            while(!(PIND &(1<<PD4)));
        }
    }

}
