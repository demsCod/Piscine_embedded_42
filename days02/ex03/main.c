#include <avr/io.h>
#include <avr/interrupt.h>
/*The USART has to be initialized before any communication can take place. The initialization process normally consists of
setting the baud rate, setting frame format and enabling the transmitter or the receiver depending on the usage. For interrupt
driven USART operation, the global interrupt flag should be cleared (and interrupts globally disabled) when doing the
initialization.
Before doing a re-initialization with changed baud rate or frame format, be sure that there are no ongoing transmissions
during the period the registers are changed. The TXCn flag can be used to check that the transmitter has completed all
transfers, and the RXC flag can be used to check that there are no unread data in the receive buffer. Note that the TXCn flag
must be cleared before each transmission (before UDRn is written) if it is used for this purpos
*/
#define BAUD 115200
#define BIT_SET(reg, target) reg |= (1 << target)
#define BIT_RESET(reg) reg = 0
#define MYUBRR MYUBRR (F_CPU/ (16 * BAUD))

void uart_init()
{
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    /*ACTIVER LES TRANSMISSION*/
    /*The USART transmitter is enabled by setting 
    the transmit enable (TXEN) bit in the UCSRnB register. */
    /*Enable receiver and transmitter */
    BIT_SET(UCSR0B, RXEN0);
    BIT_SET(UCSR0B, TXEN0);
    BIT_SET(UCSR0B, RXCIE0); // enable interupt

    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}



ISR (USART_RX_vect) 
{
    while ( !(UCSR0A & (1<<RXC0)) )
        ;
    char c = UDR0;
    if (c >= 65 && c <= 90)
        c += 32;
    else if (c >= 97 && c <= 122)
        c -= 32;
    UDR0 = c;
}


int main ()
{
    uart_init();
    sei();
    while (1)
    {}
}
