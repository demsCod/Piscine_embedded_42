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


void setup_timer()
{
    cli();
    TCCR1B |= (1 << WGM12);                // Mode CTC
    TCCR1B |= (1 << CS12) | (1 << CS10);   // Prescaler 1024
    BIT_SET(TIMSK1, (1 << OCIE1A));        // Activation de l'interruption
    OCR1A = TIME_TARGET;
    TCNT1 = 0;
    sei();
}

void setup_led()
{
    BIT_CLEAR(PORTD, LED_BLUE | LED_GREEN | LED_RED | LED_YELLOW | LED_CYAN | LED_PINK | LED_WHITE);  // Éteint toutes les LEDs
    BIT_SET(DDRD, LED_BLUE | LED_GREEN | LED_RED | LED_YELLOW | LED_CYAN | LED_PINK | LED_WHITE);      // Configure en sortie
}

ISR(TIMER1_COMPA_vect)
{
    static uint8_t color_table[7] = {LED_RED, LED_GREEN, LED_BLUE, LED_YELLOW , LED_CYAN , LED_PINK , LED_WHITE};
    static uint8_t i = 0;

    BIT_CLEAR(PORTD, LED_BLUE | LED_GREEN | LED_RED | LED_YELLOW);  // Éteint toutes les LEDs
    BIT_SET(PORTD, color_table[i % 7]);  // Allume la couleur suivante
    ++i;
}

int main()
{
    setup_timer();
    setup_led();
    while (1)
    {}
}
