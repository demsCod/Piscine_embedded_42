#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TIME_TARGET 15625  // 1 sec avec prescaler 1024
#define BAUD 115200
#define F_CPU_ACTUAL (F_CPU / 256) // If you keep the /256 prescaler
#define MYUBRR ((F_CPU_ACTUAL / (16 * BAUD)) - 1)

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
    BIT_SET(UCSR0B, RXEN0);
    BIT_SET(UCSR0B, TXEN0);
    // BIT_SET(UCSR0B, RXCIE0); // enable interrupt
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(unsigned char data)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
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
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));
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
    if (input[0] != '#')
        return;
    
    // Check if we have exactly 7 characters (#RRGGBB)
    int len = 0;
    while (input[len])
        len++;
    
    if (len != 7)
        return;
    
    // Check if all characters after # are valid hex
    for (int i = 1; i < 7; i++) {
        char c = input[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')))
            return;
    }
    
    char red_str[3] = {input[1], input[2], '\0'};
    char green_str[3] = {input[3], input[4], '\0'};
    char blue_str[3] = {input[5], input[6], '\0'};
    
    uint8_t red_value = ft_atoi_hexa(red_str);
    uint8_t green_value = ft_atoi_hexa(green_str);
    uint8_t blue_value = ft_atoi_hexa(blue_str);
    
    set_rgb(red_value, green_value, blue_value);  // Make sure parameter order matches your LED wiring
}
int i = 0;
char str[1024];

void read_input()
{
    if (UCSR0A & (1 << RXC0)) {  // Check if data is available
        char c = uart_rx();
        PORTB ^= 23;
        
        uart_tx(c);  // Echo back
        if (c != '\n' && c != '\r') {
            if (i < 1023) {  // Prevent buffer overflow
                str[i++] = c;
            }
        } else {
            str[i] = '\0';
            parse_input(str);
            i = 0;  // Reset index
        }
    }
}

// TCCR0A COM0A1 COM0A0 COM0B1 COM0B0 – – WGM01 WGM00
// TCCR0B FOC0A  FOC0B   – –   WGM02  CS02 CS01 CS00 
void init_rgb()
{
    cli();
    CLKPR |= (1 << CLKPS3); // Prescaler  256

    TCCR0A |= (1 << WGM00) | (1 << WGM01);                 // Mode  Fast PWM R0
    TCCR2A |= (1 << WGM20) | (1 << WGM21);                 // Mode  Fast PWM R2

    TCCR0A |= (1 << COM0B1) | (1 << COM0A1);  // green and red 
    TCCR2A |= (1 << COM2B1);    // blue

    TCCR0B |= (1 << CS01) | (1 << CS00);   // Prescaler 64
    TCCR2B |= (1 << CS22);  // Prescaler 64
    sei();
}

void setup_led()
{
    BIT_CLEAR(PORTD, LED_BLUE | LED_GREEN | LED_RED );  // Éteint toutes les LEDs
    BIT_SET(DDRD, LED_BLUE | LED_GREEN | LED_RED);      // Configure en sortie
}


int main()
{
    init_rgb();
    uart_init();
    setup_led();
    uart_printstr("UART Ready!\r\n");
    while(1)
    {
        UDR0 = 'A';
        read_input();
        _delay_ms(10);
    }
}