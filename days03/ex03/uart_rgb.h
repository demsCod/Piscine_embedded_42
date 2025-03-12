
#ifndef UART_RGB_H
#define UART_RGB_H



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

// Définition des LEDs
#define LED_BLUE   (1 << PD3)
#define LED_RED    (1 << PD5)
#define LED_GREEN  (1 << PD6)

void init_rgb();
void set_rgb(uint8_t r, uint8_t g, uint8_t b);


int whos_index( char *str, char c);
unsigned char	ft_atoi_hexa(char *str);
void parse_input(char *input);
void read_input();

char uart_rx(void);
void uart_tx(unsigned char c);
void uart_printstr(const char *str);

#endif
