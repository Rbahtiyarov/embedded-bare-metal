#include <stdint.h>

// Адреса регистров STM32F446 (из Reference Manual RM0390).
// Пока пишем адреса вручную — чтобы ты видела, что регистр это просто ячейка памяти.
// Позже подключим официальный заголовок ST, где все эти имена уже определены.

#define PERIPH_BASE       0x40000000UL
#define AHB1PERIPH_BASE   (PERIPH_BASE + 0x00020000UL)

// Регистр включения тактирования шины AHB1 (управляет питанием портов)
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

    // Шаг 2: настроить PA5 как выход.
    // В MODER на каждую ногу отведено 2 бита. Для PA5 это биты 10-11.
    // 01 = режим "выход". Сначала очищаем оба бита, потом ставим 01.
    GPIOA_MODER &= ~(3 << (5 * 2));   // очистить биты 10-11 (маска 0b11)
    GPIOA_MODER |=  (1 << (5 * 2));   // установить 01 -> выход

    // Шаг 3: мигаем
    while (1) {
        GPIOA_ODR ^= (1 << 5);        // инвертировать бит 5 -> переключить LD2
        delay(500000);
    }
}