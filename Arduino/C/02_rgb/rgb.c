#include <avr/io.h>
#include <util/delay.h>

#define GREEN PB5
#define BLUE PB4
#define RED PB3

int main(void) {
    DDRB |= (1 << GREEN) | (1 << RED) | (1 << BLUE);
    while (1) {
        // --- Красный ---
        PORTB |=  (GREEN);     // включить красный (бит в 1)
        _delay_ms(500);
        PORTB &= ~(1 << GREEN);     // выключить красный (бит в 0)

        // --- Зелёный ---
        PORTB |=  (1 << BLUE);
        _delay_ms(500);
        PORTB &= ~(1 << BLUE);

        // --- Синий ---
        PORTB |=  (1 << GREEN) | (1 << BLUE);
        _delay_ms(500);
        PORTB &= ~(1 << GREEN) | ~(1 << BLUE);
    }

}