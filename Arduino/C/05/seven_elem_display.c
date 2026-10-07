#include <avr/io.h>
#include <util/delay.h>

// Сегменты по пинам урока:
//   A->PD3  B->PD2  C->PD4  D->PD5  E->PD6  G->PD7   (порт D)
//   F->PB0                                            (порт B)

// Включить/выключить один сегмент по его расположению.
// Раскидано по двум портам, поэтому обходим каждый сегмент отдельно.
void set_segment(char seg, uint8_t on) {
    uint8_t bitD = 0xFF;  // какой бит порта D (0xFF = не порт D)
    uint8_t bitB = 0xFF;  // какой бит порта B

    switch (seg) {
        case 'A': bitD = PD3; break;
        case 'B': bitD = PD2; break;
        case 'C': bitD = PD4; break;
        case 'D': bitD = PD5; break;
        case 'E': bitD = PD6; break;
        case 'G': bitD = PD7; break;
        case 'F': bitB = PB0; break;
    }

    if (bitD != 0xFF) {
        if (on) PORTD |= (1 << bitD); else PORTD &= ~(1 << bitD);
    }
    if (bitB != 0xFF) {
        if (on) PORTB |= (1 << bitB); else PORTB &= ~(1 << bitB);
    }
}

// Таблица цифр: для каждой цифры — какие сегменты горят.
// Строки соответствуют сегментам A,B,C,D,E,F,G (1 = горит).
const uint8_t digits[10][7] = {
    // A  B  C  D  E  F  G
    { 1, 1, 1, 1, 1, 1, 0 }, // 0
    { 0, 1, 1, 0, 0, 0, 0 }, // 1
    { 1, 1, 0, 1, 1, 0, 1 }, // 2
    { 1, 1, 1, 1, 0, 0, 1 }, // 3
    { 0, 1, 1, 0, 0, 1, 1 }, // 4
    { 1, 0, 1, 1, 0, 1, 1 }, // 5
    { 1, 0, 1, 1, 1, 1, 1 }, // 6
    { 1, 1, 1, 0, 0, 0, 0 }, // 7
    { 1, 1, 1, 1, 1, 1, 1 }, // 8
    { 1, 1, 1, 1, 0, 1, 1 }, // 9
};

const char seg_names[7] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' };

void show_digit(uint8_t d) {
    for (uint8_t s = 0; s < 7; s++) {
        set_segment(seg_names[s], digits[d][s]);
    }
}

int main(void) {
    // Сегменты порта D (PD2-PD7) как выходы
    DDRD |= (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);
    // Сегмент F на порту B
    DDRB |= (1 << PB0);

    uint8_t d = 0;
    while (1) {
        show_digit(d);       // показать цифру
        _delay_ms(1000);
        d++;
        if (d > 9) d = 0;    // счёт 0..9 по кругу
    }
}