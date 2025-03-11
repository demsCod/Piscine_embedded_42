/* Vous devez écrire un programme qui change l’état de la LED D1 lorsque l’on
appuie sur le bouton SW1.
• Vous devez utiliser les interruptions pour lire l’état du bouton. Il est interdit de
lire les registres PINx*/


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



void set_interupt()
{
    cli();
    // SREG = 128; /* restore SREG value (I-bit) */  //10000000
    /*When the PCIE2 bit is set (one) and the I-bit in the status register (SREG) is set (one), pin change interrupt 2 is enabled. Any
    change on any enabled PCINT23..16 pin will cause an interrupt. The corresponding interrupt of pin change interrupt request
    is executed from the PCI2 interrupt vector. PCINT23..16 pins are enabled individually by the PCMSK2 register*/
    
    //PD2 (INT0/PCINT18)
    //PCIFR – Pin Change Interrupt Flag Register
    
    /*When a logic change on any PCINT23..16 pin triggers an interrupt request, PCIF2 becomes set (one). If the I-bit in SREG
    and the PCIE2 bit in PCICR are set (one), the MCU will jump to the corresponding interrupt vector. The flag is cleared when
    the interrupt routine is executed. Alternatively, the flag can be cleared by writing a logical one to it*/
    
    //PCMSK2 – Pin Change Mask Register 2
    
    /* Bit 7..0 – PCINT23..16: Pin Change Enable Mask 23..16
    Each PCINT23..16-bit selects whether pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is set
    and the PCIE2 bit in PCICR is set, pin change interrupt is enabled on the corresponding I/O pin. If PCINT23..16 is cleared,
    pin change interrupt on the corresponding I/O pin is disabled*/
    // //PCICR – Pin Change Interrupt Control Register
    PCICR |= (1 << PCIE2);
    PCIFR |= (1 << PCIF2);
    PCMSK2 |= (1 << PCINT18);
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
    

    set_interupt();
    while(1)
    {
        _delay_ms(50);
        if(is_press == 0)
        {
            if (is_press == 0)
                PORTB ^= (1 << PB0);
            while(!is_press)
            {}
        }
    }
}
