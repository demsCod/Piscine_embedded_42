#include <avr/io.h>

int main()
{
    DDRB = 1; // définir la direction (DDRB =  0001) D1
    while (1)
        PORTB = 1;
}   