
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

#define PIXEL_STROBE 1000 /*μs*/
#define PIXEL_DELAY  100  /*μs*/

void col_strobe() {
    PORTF &= ~(1<<6);
    _delay_us(PIXEL_STROBE);
    PORTF |=   1<<6;
    _delay_us(PIXEL_STROBE);
}

uint8_t fp2800_lut[28] = { 0x20, 0x04, 0x24, 0x08, 0x28, 0x0C, 0x2C,
                           0x21, 0x05, 0x25, 0x09, 0x29, 0x0D, 0x2D,
                           0x22, 0x06, 0x26, 0x0A, 0x2A, 0x0E, 0x2E,
                           0x23, 0x07, 0x27, 0x0B, 0x2B, 0x0F, 0x2F };

void col_select(uint8_t col, uint8_t val) {
    PORTF |= 0x3F;
    PORTF &= ~fp2800_lut[col];
    PORTF &= ~((!!val)<<4);
}

void row_select(uint8_t row, uint8_t val) {
    PORTA &= 0xC0;
    PORTC &= 0x03;
    if (val)
        PORTC |= 0x80 >> row;
    else
        PORTA |= 0x01 << row;
}

int main(void) {
    DDRA |= 0x3F;
    DDRC |= 0xFC;
    DDRF |= 0x7F;

    PORTF |= 1<<6;
    PORTF |= 0x3F;

    uint8_t sval = 0;
    while (42) {
        uint8_t val = sval;
        for (uint8_t row = 0; row<6; row++) {
            row_select(row, val);
            for (uint8_t col = 0; col<28; col++) {
                col_select(col, val);
                col_strobe();
                val = !val;
            }
        }
        sval = !sval;
    }
}
