#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TOS  324.31
#define K    1.22

#define TIME_TARGET 15625  // 1 sec avec prescaler 1024
#define BAUD 115200
#define F_CPU_ACTUAL (F_CPU / 256) // If you keep the /256 prescaler
#define MYUBRR ((F_CPU / (16 * BAUD)))

#define BIT_RESET(reg)    ((reg) = 0)
#define BIT_SET(reg, mask)    ((reg) |= (mask))
#define BIT_CLEAR(reg, mask)  ((reg) &= ~(mask))
#define BIT_TOGGLE(reg, mask) ((reg) ^= (mask))

#include <avr/io.h>
#include <stdbool.h>

char num[4];

#include <avr/io.h>

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
    
    const char digits[] = "0123456789";
    
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
    ADMUX = (1 << REFS0) | (1 << REFS1) | (1 << MUX3);
    // ADMUX |= 3; 
    ADCSRA |= (1 << ADEN)  | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void take_value(void)
{
   
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    long adc_value = (ADC - TOS) / K;
    itoa_base(adc_value, num, 10);
    uart_printstr(num);
}

int main()
{
    uart_init();
    setup_adc();
    while(1)
    {
        take_value();
        uart_printstr("\r\n");
        _delay_ms(20);
    }
}