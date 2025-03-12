#include <avr/io.h>
#include <avr/interrupt.h>

// PB1 == OC1A (PWM)
volatile uint8_t interrupt_counter = 0;
volatile uint8_t cycle = 0;
volatile uint8_t direction = 1; // 1 = montée, 0 = descente

// ⚙️ Configurer Timer0 en mode CTC (Interruption toutes les ~4ms)
void set_CTC()
{
    cli();  // Désactiver les interruptions globales

    // Mode CTC (Clear Timer on Compare Match)
    TCCR0A = (1 << WGM01);

    // Prescaler = 256 → Fréquence = 16MHz / 256 / 250 ≈ 250 Hz (~4ms)
    TCCR0B = (1 << CS12);

    TCNT0 = 0;    // Reset du compteur
    OCR0A = 250;  // Comparateur pour l'interruption (~4ms)

    TIMSK0 |= (1 << OCIE0A); // Activer l'interruption sur OCR0A

    sei();  // Réactiver les interruptions globales
}

// ⚙️ Configurer Timer1 en mode Fast PWM 8 bits sur PB1 (OC1A)
void setup_PWM()
{
    // Configurer PB1 en sortie
    DDRB |= (1 << PB1);

    // Fast PWM 8 bits (WGM10 = 1, WGM11 = 0, WGM12 = 1, WGM13 = 0)
    TCCR1A = (1 << WGM10) | (1 << COM1A1);
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // Prescaler = 64

    OCR1A = 0; // Démarrer avec la LED éteinte
}


ISR(TIMER0_COMPA_vect)
{
    interrupt_counter++;

    if (interrupt_counter >= 125)  // 0.5 seconde écoulée (250 * 4ms ≈ 1s)
    {
        interrupt_counter = 0;
        direction = !direction; // Inverser montée/descente
    }

    if (direction) // Montée
    {
        if (cycle < 255) cycle += 3; // Monter doucement
    }
    else // Descente
    {
        if (cycle > 0) cycle -= 3; // Descendre doucement
    }

    OCR1A = cycle; // Mise à jour du PWM
}

int main()
{
    set_CTC();  // Activer le Timer0 pour les interruptions
    setup_PWM(); // Configurer le PWM sur Timer1

    while (1)
    {
        // Boucle infinie, l'ISR gère tout
    }
}
