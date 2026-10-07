#include <avr/io.h>
#include <util/delay.h>

// RGB на ШИМ-выходах:
//   красный -> пин 9  -> PB1 -> OC1A (Timer1)
//   зелёный -> пин 10 -> PB2 -> OC1B (Timer1)
//   синий   -> пин 11 -> PB3 -> OC2A (Timer2)

void pwm_init(void) {
    // --- Ноги как выходы ---
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);

    // === Timer1 (16-бит) -> каналы A и B, красный и зелёный ===
    // Fast PWM, 8-битный (TOP = 0xFF): WGM10=1, WGM12=1
    // Неинвертирующий выход на OC1A и OC1B: COM1A1=1, COM1B1=1
    TCCR1A = (1 << WGM10)                    // Fast PWM 8-bit (часть 1)
           | (1 << COM1A1)                   // OC1A: сброс при совпадении
           | (1 << COM1B1);                  // OC1B: сброс при совпадении
    TCCR1B = (1 << WGM12)                    // Fast PWM 8-bit (часть 2)
           | (1 << CS11);                    // предделитель = 8 -> ~7.8 кГц

    // === Timer2 (8-бит) -> канал A, синий ===
    // Fast PWM (TOP = 0xFF): WGM20=1, WGM21=1
    // Неинвертирующий выход на OC2A: COM2A1=1
    TCCR2A = (1 << WGM20) | (1 << WGM21)     // Fast PWM
           | (1 << COM2A1);                  // OC2A: сброс при совпадении
    TCCR2B = (1 << CS21);                    // предделитель = 8
}

// Установить яркость каналов (0 = выкл, 255 = максимум)
void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
    OCR1A = r;   // красный
    OCR1B = g;   // зелёный
    OCR2A = b;   // синий
}

int main(void) {
    pwm_init();

    uint8_t i;
    while (1) {
        // Красный -> Зелёный
        for (i = 0; i < 255; i++) { set_rgb(255 - i, i, 0); _delay_ms(8); }
        // Зелёный -> Синий
        for (i = 0; i < 255; i++) { set_rgb(0, 255 - i, i); _delay_ms(8); }
        // Синий -> Красный
        for (i = 0; i < 255; i++) { set_rgb(i, 0, 255 - i); _delay_ms(8); }
    }
}