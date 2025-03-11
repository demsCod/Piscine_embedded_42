#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TIME_TARGET 15625  // 1 sec avec prescaler 1024

#define BIT_RESET(reg)    ((reg) = 0)
#define BIT_SET(reg, mask)    ((reg) |= (mask))
#define BIT_CLEAR(reg, mask)  ((reg) &= ~(mask))
#define BIT_TOGGLE(reg, mask) ((reg) ^= (mask))

// Définition des LEDs
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
    CLKPR |= (1 << CLKPS3); // 256
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


void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0A =r;
   OCR0B = g;
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


int main()
{
    setup_led();
    init_rgb();
    unsigned char i = 90;
    while(1)
    {
       _delay_ms(100);
       wheel(i);
       i--;




    }
}