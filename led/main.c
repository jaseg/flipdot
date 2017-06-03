
#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>


#define PULSE_DURATION  500 /*us*/
#define SETTLE_DURATION 500 /*us*/
#define COL_DURATION    1   /*ms*/


void select_col(uint8_t col) {
    PORTE &= ~((1<<3) | (1<<4) | (1<<5));
    PORTH &= ~(1<<3);
    PORTG &= ~(1<<5);

    PORTE |= (col&1)<<4;
    PORTE |= ((col>>1)&1)<<5;

    PORTG |= ((col>>2)&1)<<5;
    PORTE |= ((col>>3)&1)<<3;
    PORTH |= ((col>>4)&1)<<3;
}

void shift_clk(void) {
    PORTH &= ~(1<<5);
    _delay_us(PULSE_DURATION);
    PORTH |=   1<<5;
}

void shift_strobe(void) {
    PORTH &= ~(1<<4);
    _delay_us(PULSE_DURATION);
    PORTH |=   1<<4;
}

void shift_data(uint8_t *data) {
    for (uint8_t byte=0; byte<5; byte++) {
        for (uint8_t val=data[byte], i=0; i<8; val>>=1, i++) {
            PORTH &= ~(1<<6);
            PORTH |= (val&1) << 6;
            _delay_us(SETTLE_DURATION);
        }
        shift_clk();
    }
    shift_strobe();
}

int main(void) {
    DDRB |= (1<<4) | (1<<5) | (1<<6) | (1<<7);
    DDRE |= (1<<3) | (1<<4) | (1<<5);
    DDRF |= (1<<0) | (1<<1) | (1<<2);
    DDRG |= (1<<5);
    DDRH |= (1<<3) | (1<<4) | (1<<5) | (1<<6);

    uint8_t col=0;
    uint8_t rowdata[4] = {1, 1, 1, 1};
    while (23) {
        select_col(col);
        col = (col+1) & 0x1F;

        if (!col) {
            rowdata[0] <<= 1;
            if (!rowdata[0])
                rowdata[0] = rowdata[1] = rowdata[2] = rowdata[3] = 1;
            else
                rowdata[1] = rowdata[2] = rowdata[3] = rowdata[0];
        }

        shift_data(rowdata);

        _delay_ms(COL_DURATION);
    }
}
