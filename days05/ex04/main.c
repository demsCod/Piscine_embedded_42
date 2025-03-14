#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TIME_TARGET 15625  // 1 sec avec prescaler 1024
#define BAUD 115200
#define F_CPU_ACTUAL (F_CPU / 256) // If you keep the /256 prescaler
#define MYUBRR ((F_CPU / (16 * BAUD)))

#define BIT_RESET(reg)    ((reg) = 0)
#define BIT_SET(reg, mask)    ((reg) |= (mask))
#define BIT_CLEAR(reg, mask)  ((reg) &= ~(mask))
#define BIT_TOGGLE(reg, mask) ((reg) ^= (mask))


char num[3];


#define LED_BLUE   (1 << PD3)
#define LED_RED    (1 << PD5)
#define LED_GREEN  (1 << PD6)
#define LED_YELLOW (LED_GREEN | LED_RED)
#define LED_CYAN (LED_GREEN | LED_BLUE)
#define LED_PINK (LED_BLUE | LED_RED)
#define LED_WHITE (LED_GREEN | LED_RED | LED_BLUE)


// TCCR0A COM0A1 COM0A0 COM0B1 COM0B0 – – WGM01 WGM00
// TCCR0B FOC0A  FOC0B   – –   WGM02  CS02 CS01 CS00 
void init_rgb()
{
    cli();
    CLKPR |= (1 << CLKPS3); // Prescaler  256
    TCCR0A |= (1 << WGM00) | (1 << WGM01);                 // Mode  Fast PWM
    TCCR2A |= (1 << WGM20) | (1 << WGM21);                 // Mode  Fast PWM


    //Clear OC0B on compare match, set OC0B at BOTTOM, (non-inverting mode)
    TCCR0A |= (1 << COM0B1) | (1 << COM0A1);    
    
    TCCR2A |= (1 << COM2B1);             
    

    TCCR0B |= (1 << CS01) | (1 << CS00);   // Prescaler 64
    TCCR2B |= (1 << CS22) | (1 << CS20);  // Prescaler 64
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


void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
   OCR0A = g;
   OCR0B = r;
   OCR2B = b;
}

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




void reverse(char *str, uint8_t length) {
    uint8_t start = 0;
    uint8_t end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void itoa_base(int16_t value, char *buffer, uint8_t base) {
    if (base < 2 || base > 16) {
        buffer[0] = '\0'; // Base invalide
        return;
    }
    
    uint8_t is_negative = 0;
    uint8_t i = 0;
    
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }
    
    if (value < 0 && base == 10) {
        is_negative = 1;
        value = -value;
    }
    
    const char digits[] = "0123456789abcdef";
    
    while (value > 0) {
        buffer[i++] = digits[value % base];
        value /= base;
    }
    
    if (is_negative) {
        buffer[i++] = '-';
    }
    
    buffer[i] = '\0';
    reverse(buffer, i);
}

void uart_init()
{
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    BIT_SET(UCSR0B, (1 << RXEN0));
    BIT_SET(UCSR0B, (1 << TXEN0));
    // BIT_SET(UCSR0B, RXCIE0); // enable interrupt
}

void uart_tx(unsigned char data)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void uart_printstr(const char *str)
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

void setup_adc()
{
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

}

int main()
{
    uart_init();
    setup_adc();
    init_rgb();
    setup_led();
    while(1)
    {
        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC));
        uint16_t adc_value = ADC;
        itoa_base(adc_value, num, 10);
        uart_printstr(num);
        uart_printstr("\n\r");
        wheel(adc_value / 4);
        _delay_ms(70);


    }
}