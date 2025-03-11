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
    //IF ITS A ARROW KEY
    if (c == 27)
    {
        uart_rx();
        c = uart_rx();
        if (c == 65)
        {
            return 3;
        }
        else if (c == 66)
        {
            return 3;
        }
        else if (c == 67)
        {
            return 3;
        }
        else if (c == 68)
        {
            return 3;
        }
    }
    if (input_flag == 0)
    {

        if (c == 127 || c == '\b') // Touche backspace
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
        if (i_ps >= 20)
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
	return (s1[i] - s2[i]);
}

void clear_string(char *str)
{
    while (*str)
    {
        *str = '\0';
        str++;
    }
}

void handle_user()
{
    uart_printstr(x.username);
    uart_printstr("--");
    uart_printstr(x.password);
    uart_printstr("\r\n");
    if (ft_strcmp(x.user_case, "DEMS42") == 0 && ft_strcmp(x.psw_case, "HACKING") == 0)
    {
        uart_printstr("DEMS42");
        uart_printstr(" Logged in\r\n");
        PORTB |= 23;
        for(int i = 0; i < 8; i++)
        {
            _delay_ms(100);
            PORTB ^= 23;
            PORTD ^= (1 << PD3);

        }
        PORTB ^= 23;
        PORTD &= ~(1 << PD3);
        _delay_ms(50);
        PORTD |= (1 << PD6);
        clear_string(x.psw_case);   
        uart_printstr(WELCOME_PROMPT);
        clear_string(x.user_case);
        i_ps = 0;
        i_us = 0;
        uart_printstr("Do you want to log out ?\r\n Press any key to log out\r\n"); 
        input_flag = 3;
    }
    else
    {
        uart_printstr("Bad Password, Try again\r\n");
        PORTB |= 23;
        PORTD &= ~(1 << PD3);
        PORTD |= (1 << PD5);
        clear_string(x.psw_case);
        clear_string(x.user_case);
        i_ps = 0;
        i_us = 0;
        input_flag = 0;
        _delay_ms(500);
        PORTB &= ~23;
        PORTD &= ~(1 << PD5);
        uart_printstr(US_PROMPT);
        PORTD |= (1 << PD3);
    }
}

ISR(TIMER1_COMPA_vect)
{
    char c = uart_rx(); // Lire une seule fois
    PORTB |= 1;  
    int result = handle_input(c);
    if (input_flag == 3)
    {
        clear_string(x.psw_case);
        clear_string(x.user_case);
        i_ps = 0;
        i_us = 0;
        uart_printstr(US_PROMPT);
        input_flag = 0;
        PORTB &= ~23;
        PORTD &= ~(1 << PD6);
        PORTD |= (1 << PD3);
        return;
    }

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
    else if (result == 59)
    {
        uart_printstr("\r\n Max length reached\r\n");
        _delay_ms(500);
        PORTB |= 23;
        clear_string(x.psw_case);
        clear_string(x.user_case);
        i_ps = 0;
        i_us = 0;
        PORTB &= ~23;
        uart_printstr(US_PROMPT);
        _delay_ms(1000);
        input_flag = 0;
        uart_rx();
        uart_rx();
        uart_rx();
    }
    PORTB &= ~1;
}


int main ()
{
    DDRB |= 23;
    DDRB |= (1 << PB0);
    DDRD |= (1 << PD6) | (1 << PD5) | (1 << PD3);
    PORTD |= (1 << PD3);
    uart_init();
    init_interface("Usr1", "Psw1", x);
    uart_printstr(US_PROMPT);
    setup_timer_1();
    while(1)
    {};
    
}