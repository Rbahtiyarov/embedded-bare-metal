#include <stdint.h>

#define PERIPH_BASE       0x40000000UL
#define AHB1PERIPH_BASE   (PERIPH_BASE + 0x00020000UL)

#define RCC_BASE          (AHB1PERIPH_BASE + 0x3800UL)
#define RCC_AHB1ENR       (*(volatile uint32_t *)(RCC_BASE + 0x30UL))

// Регистры порта A
#define GPIOA_BASE        (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOA_MODER       (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_ODR         (*(volatile uint32_t *)(GPIOA_BASE + 0x14UL))

// Простая задержка (пока грубая, «на глаз» — точную сделаем через таймер позже)
static void delay(volatile uint32_t count) {
    while (count--) { }
}

int main(void) {
    // Шаг 1: включить тактирование порта A (бит 0 в AHB1ENR).
    // БЕЗ этой строки порт A не работает вообще — главное отличие от AVR!
    RCC_AHB1ENR |= (1 << 0);

    // Шаг 2: 
    //Красный (R) модуля →  PA9 (пин D8 на Arduino-разъёме)
    //Зелёный (G) → PA6 (D12)
    //Синий (B) → PA7 (D11)
    //Общий вывод (- / GND / S) модуля → GND Nucleo
    // В MODER на каждую ногу отведено 2 бита.
    // 01 = режим "выход". Сначала очищаем оба бита, потом ставим 01.
    GPIOA_MODER &= ~( (3 << (9 * 2)) | (3 << (6 * 2)) | (3 << (7 * 2)));   // очистить биты 10-11 (маска 0b11)
    GPIOA_MODER |=  (1 << (9 * 2)) | (1 << (6 * 2)) | (1 << (7 * 2));   // установить 01 -> выход

    // Шаг 3: мигаем
    while (1) {
        GPIOA_ODR |= (1 << 9);        // инвертировать бит 9 -> переключить LD2
        delay(500000);
        GPIOA_ODR |= (1 << 6);
        delay(500000);
        GPIOA_ODR |= (1 << 7);
        delay(500000);
        GPIOA_ODR &= ~(1 << 9);   // выключить красный
        delay(500000);
        GPIOA_ODR &= ~(1 << 6);   // выключить зелёный
        delay(500000);
        GPIOA_ODR &= ~(1 << 7);   // выключить синий
        delay(500000);
    }
}