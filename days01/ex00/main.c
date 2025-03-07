#include <avr/io.h>

void delay(unsigned long time)
{

   for (volatile int i = 0; i < time; i++)
   {
        for (volatile int i = 0; i < 1000; i++)
            ;
   }
}

int main ()
{
    DDRB |= 2;
    while (1)
    {
        PORTB ^= (1 << PB1);
        delay(500);
    }
}