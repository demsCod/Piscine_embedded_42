#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "inter_user.h"

user_t x;

int input_flag = 0;



void setup_timer_1()
{
    cli();
    BIT_SET(TCCR1B, WGM12);
    BIT_SET(TCCR1B, CS12);
    BIT_SET(TCCR1B, CS10);
    BIT_SET(TIMSK1, OCIE1A);
    OCR1A = 15;
    TCNT1 = 0;
    sei();
}



char uart_rx(void)
{
    while ( !(UCSR0A & (1<<RXC0)) )
        ;
    return UDR0;
}

int i_us = 0; // Déclaration explicite
int i_ps = 0;

int handle_input(char c)
{
    if (input_flag == 0)
    {

        if (c == 127) // Touche backspace
        {
            if (i_us > 0)
            {
                x.user_case[i_us] = '\0';
                i_us--;
                return 1; // Indique qu'un caractère a été supprimé
            }
            return 4;
        }
        else if (c == '\n' || c == '\r') // Fin de commande
        {
            x.user_case[i_us] = '\0';
            return 2;
        }
        if (i_us >= 49)
            return 59;
        x.user_case[i_us] = c;
        i_us++;
        return 0;
    }
    else
    {
        if (c == 127) // Touche backspace
        {
            if (i_ps > 0)
            {
                x.psw_case[i_ps] = '\0';
                i_ps--;
                return 1; // Indique qu'un caractère a été supprimé
            }
            return 4;
        }
        else if (c == '\n' || c == '\r') // Fin de commande
        {
            x.psw_case[i_ps] = '\0';
            return 2;
        }
        if (i_ps >= 49)
            return 59;
        x.psw_case[i_ps] = c;
        i_ps++;
        return 0;
    }

}


int	ft_strcmp(char *s1, char *s2)
{
	unsigned int	i;

	i = 0;
	while ((s1[i] == s2[i]) && s1[i] != '\0')
	{
		i++;
	}
	if (s1[i] != s2[i])
		return (s1[i] - s2[i]);
	return (0);
}

void clear_string(char *str)
{
    
}
void handle_user()
{
    if (ft_strcmp(x.user_case, x.username) == 0 && ft_strcmp(x.psw_case, x.password) == 0)
    {
        uart_printstr(WELCOME_PROMPT);
        cli();
    }
    else
    {
        uart_printstr("Bad Password, Try again\r\n");
        PORTB |= 23;
    }
}

ISR(TIMER1_COMPA_vect)
{
    char c = uart_rx(); // Lire une seule fois
    DDRB |= 23;
    int result = handle_input(c);

    if (result == 1)
    {
        uart_printstr("\b \b"); // Efface un caractère
    }
    else if (result == 2)
    {
        uart_printstr("\r\n");
        if (input_flag == 0)
        {
            uart_printstr(PS_PROMPT);
            input_flag = 1;
        }
        else
        {
            handle_user();
        }
    }
    else if (result == 0)
    {
        if (input_flag)
            uart_tx('*');
        else
            uart_tx(c);
    }
}


int main ()
{
    uart_init();
    init_interface("Usr1", "Psw1", x);
    uart_printstr(US_PROMPT);
    setup_timer_1();
    while(1)
    {};
    
}