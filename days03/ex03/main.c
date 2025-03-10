#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TIME_TARGET 15625  // 1 sec avec prescaler 1024
#define BAUD 115200
#define MYUBRR (F_CPU/ (16 * BAUD))

#define BIT_RESET(reg)    ((reg) = 0)
#define BIT_SET(reg, mask)    ((reg) |= (mask))
#define BIT_CLEAR(reg, mask)  ((reg) &= ~(mask))
#define BIT_TOGGLE(reg, mask) ((reg) ^= (mask))

// Définition des LEDs
#define LED_BLUE   (1 << PD3)
#define LED_RED    (1 << PD5)
#define LED_GREEN  (1 << PD6)


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

void uart_tx(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)))
        ;
    UDR0 = data;
}

void uart_printstr(const char* str)
{
    while(*str)
    {
        uart_tx(*str);
        _delay_ms(5);
        ++str;
    }
}

char uart_rx(void)
{
    while ( !(UCSR0A & (1<<RXC0)) )
        ;
    return UDR0;
}



int whos_index( char *str, char c)
{
    int i = 0;
    while(str[i] && str[i] != c)
    i++;
    return i;
}
char hexa[] = {"0123456789abcdef"};

unsigned char	ft_atoi_hexa(char *str)
{
	unsigned char  num;
	int i;
    
    
	i = 0;
	num = 0;
	while ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f'))
	{
        num = (num * 16 ) + (whos_index(hexa, str[i]));
		i++;
	}
	return (num);
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0A = g;
    OCR0B = r;
    OCR2B = b;
}
void parse_input(char *input)
{
    
    if (input[0] && input[0] != '#')
    return;
    int i = 1;
    while (input[i])
    {
        if (whos_index(hexa, input[i]) > 16)
                return;
            i++;
    }
    if (i != 7)
        return;
    char red_str[2];
    red_str[0] = input[1];
    red_str[1] = input[2]; 
    uint8_t  red_value = ft_atoi_hexa(red_str);
    char green_str[3];
    green_str[0] = input[3];
    green_str[1] = input[4]; 
    green_str[2] = '\0';
    uint8_t green_value = ft_atoi_hexa(green_str);
    char blue_str[3];
    blue_str[0] = input[5];
    blue_str[1] = input[6]; 
    blue_str[2]  = '\0';
    uint8_t blue_value = ft_atoi_hexa(blue_str);
    if (blue_value == 0 && green_value == 0)
        PORTB ^= 23;
    set_rgb(red_value, blue_value, green_value);
}

int i = 0;
char str[1024];

void read_input()
{
    char c = uart_rx(); // Lire une seule fois
    uart_tx(c);
    PORTB ^= 23;
    if (c != '\n' && c != '\r')
        str[i++] = c;
    else
    {
        str[i] = '\0';
        parse_input(str);
        str[0] = '\0';
    }

}

// TCCR0A COM0A1 COM0A0 COM0B1 COM0B0 – – WGM01 WGM00
// TCCR0B FOC0A  FOC0B   – –   WGM02  CS02 CS01 CS00 
void init_rgb()
{
    cli();
   // CLKPR |= (1 << CLKPS3); // 256
    TCCR0A |= (1 << WGM00) | (1 << WGM01);                 // Mode  Fast PWM
    TCCR2A |= (1 << WGM20) | (1 << WGM21);                 // Mode  Fast PWM

    ///TCCR0B |= (1 << WGM02); // counting sequence OCRA

    //Clear OC0B on compare match, set OC0B at BOTTOM, (non-inverting mode)
    TCCR0A |= (1 << COM0B1) | (1 << COM0A1);    
    
    TCCR2A |= (1 << COM2B1);             
    

    TCCR0B |= (1 << CS01) | (1 << CS00);   // Prescaler 64
    TCCR2B |= (1 << CS22);  // Prescaler 64
    //BIT_SET(TIMSK1, (1 << OCIE0B));        // Activation de l'interruption
    //OCR0B = 255;
    //OCR0A = 255;
    sei();
}

void setup_led()
{
    BIT_CLEAR(PORTD, LED_BLUE | LED_GREEN | LED_RED );  // Éteint toutes les LEDs
    BIT_SET(DDRD, LED_BLUE | LED_GREEN | LED_RED);      // Configure en sortie
}

/*ISR(TIMER1_COMPA_vect)
{
    static uint8_t color_table[7] = {LED_RED, LED_GREEN, LED_BLUE, LED_YELLOW , LED_CYAN , LED_PINK , LED_WHITE};
    static uint8_t i = 0;

    BIT_CLEAR(PORTD, LED_BLUE | LED_GREEN | LED_RED | LED_YELLOW);  // Éteint toutes les LEDs
    BIT_SET(PORTD, color_table[i % 7]);  // Allume la couleur suivante
    ++i;
}*/



void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } 
    else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } 
    else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}


int main()
{
    uart_init();
    init_rgb();
    _delay_ms(100);
    setup_led();
    uart_printstr("UART Ready!\r\n");
    while(1)
    {

        read_input();
        _delay_ms(10);
    }
}