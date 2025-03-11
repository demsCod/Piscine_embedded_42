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
#define MYUBRR (F_CPU/ (16 * BAUD))
#define BIT_SET(reg, target) reg |= (1 << target)
#define BIT_RESET(reg) reg = 0


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
}


void uart_tx(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)))
        ;
    UDR0 = data;
}

void setup_timer_1()
{
    cli();
    BIT_SET(TCCR1B, WGM12);
    BIT_SET(TCCR1B, CS12);
    BIT_SET(TCCR1B, CS10);
    BIT_SET(TIMSK1, OCIE1A);
    OCR1A = 15625;
    TCNT1 = 0;
    sei();
}

ISR (TIMER1_COMPA_vect)
{
    uart_tx('Z');
}

int main ()
{
    uart_init();
    setup_timer_1();
    while (1)
    {}
}
