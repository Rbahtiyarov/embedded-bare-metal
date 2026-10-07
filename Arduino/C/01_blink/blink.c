#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= (1 << PB4);        // PB5 (пин 13, светодиод "L") — выход

    while (1) {
        PORTB ^= (1 << PB4);   // инвертируем состояние вывода
        _delay_ms(1000);
    }
}