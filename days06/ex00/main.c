#include <avr/io.h>
#include <util/twi.h>
//I2C Baud Rate = CPU Clock Frequency / (16 + 2 * TWBR * Prescaler)
#define F_SCL (F_CPU / (16 + 2 * TWBR)); // Fréquence I2C = 100 kHz
#define TWBR_VALUE ((F_CPU / F_SCL - 16) / 2)  // Calcul de TWBR

void i2c_init(void) {
    TWSR = 0x00;  // Prescaler = 1
    TWBR = (uint8_t)TWBR_VALUE;  // Configuration du registre TWBR
}

void i2c_start(void) {

}