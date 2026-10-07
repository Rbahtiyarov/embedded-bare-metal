#include <stdint.h>

// Границы секций из линкер-скрипта
extern uint32_t _sdata, _edata, _sbss, _ebss, _sidata;
extern uint32_t _estack;

int main(void);          // твой main из blink.c

// Обработчик сброса: выполняется самым первым при включении/reset
void Reset_Handler(void) {
    // Копируем инициализированные данные (.data) из flash в RAM
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) { *dst++ = *src++; }

    // Обнуляем секцию .bss (неинициализированные глобальные переменные)
    dst = &_sbss;
    while (dst < &_ebss) { *dst++ = 0; }

    main();                 // запускаем основную программу
    while (1) { }           // на всякий случай, если main вернётся
}

// Заглушка для любого необработанного прерывания
void Default_Handler(void) { while (1) { } }

// Таблица векторов прерываний — кладётся в самое начало flash.
// Первый элемент = вершина стека, второй = адрес Reset_Handler.
__attribute__((section(".isr_vector")))
uint32_t *vector_table[] = {
    (uint32_t *)&_estack,        // 0: начальное значение указателя стека
    (uint32_t *)Reset_Handler,   // 1: адрес обработчика сброса
    // Остальные векторы пока не нужны для blink
};