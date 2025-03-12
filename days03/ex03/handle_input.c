#include "uart_rgb.h"

int whos_index( char *str, char c)
{
    int i = 0;
    while(str[i] && str[i] != c)
    i++;
    return i;
}

int i = 0;
char str[1024];



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
            uart_printstr("\r\n");
        }
    }
}