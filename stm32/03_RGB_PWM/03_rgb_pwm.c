#include <stdint.h>

#define PERIPH_BASE       0x40000000UL
#define AHB1PERIPH_BASE   (PERIPH_BASE + 0x00020000UL)

#define RCC_BASE          (AHB1PERIPH_BASE + 0x3800UL)
#define RCC_AHB1ENR       (*(volatile uint32_t *)(RCC_BASE + 0x30UL))
#define RCC_APB1ENR       (*(volatile uint32_t *)(RCC_BASE + 0x40UL))

// Регистры порта A
#define GPIOA_BASE        (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOA_MODER       (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_ODR         (*(volatile uint32_t *)(GPIOA_BASE + 0x14UL))

// Регистры порта C
#define GPIOC_BASE        (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOC_MODER       (*(volatile uint32_t *)(GPIOC_BASE + 0x00UL))
#define GPIOC_ODR         (*(volatile uint32_t *)(GPIOC_BASE + 0x14UL))

//Объявление регистров AFR
#define GPIOA_AFRL        (*(volatile uint32_t *)(GPIOA_BASE + 0x20UL))
#define GPIOC_AFRH        (*(volatile uint32_t *)(GPIOC_BASE + 0x24UL))

//TIM3
#define TIM3_BASE         0x40000400UL
#define TIM3_PSC          (*(volatile uint32_t *)(TIM3_BASE + 0x28UL))
#define TIM3_ARR          (*(volatile uint32_t *)(TIM3_BASE +  0x2CUL))
//CCMR
#define TIM3_CCMR1        (*(volatile uint32_t *)(TIM3_BASE + 0x18UL))
#define TIM3_CCMR2        (*(volatile uint32_t *)(TIM3_BASE + 0x1CUL))

//CCER
#define TIM3_CCER        (*(volatile uint32_t *)(TIM3_BASE + 0x20UL))

#define TIM3_CR1  (*(volatile uint32_t *)(TIM3_BASE + 0x00UL))
#define TIM3_CCR1 (*(volatile uint32_t *)(TIM3_BASE + 0x34UL))
#define TIM3_CCR2 (*(volatile uint32_t *)(TIM3_BASE + 0x38UL))
#define TIM3_CCR3 (*(volatile uint32_t *)(TIM3_BASE + 0x3CUL))

static void delay(volatile uint32_t count) {
    while (count--) { }
}

int main(void) {
    // Включить тактирование для таймера TIM3
    RCC_APB1ENR |= (1 << 1);
    // Включить тактирование порта A и C(бит 0 и 2 в AHB1ENR).
    RCC_AHB1ENR |= ((1 << 0) | (1 << 2));
    // Шаг 2: 
    //Красный (R) модуля →  PC8
    //Зелёный (G) → PA6 (D12)
    //Синий (B) → PA7 (D11)
    //Общий вывод (- / GND / S) модуля → GND Nucleo
    
    // В MODER на каждую ногу отведено 2 бита.
    // 10 = режим "альтернативный"
    // MODER: режим "10" (альт. функция) = значение 2
    // Порт A: только PA6, PA7
    GPIOA_MODER &= ~( (3 << (6*2)) | (3 << (7*2)) );   // очистить
    GPIOA_MODER |=  ( (2 << (6*2)) | (2 << (7*2)) );   // 10 = альт.функция
    // Порт C: PC8
    GPIOC_MODER &= ~( 3 << (8*2) );                    // очистить
    GPIOC_MODER |=  ( 2 << (8*2) );                    // 10

    // AFR: записать AF2 (значение 2), 4 бита на ногу
    GPIOA_AFRL |= (2 << (6*4)) | (2 << (7*4));         // PA6, PA7
    GPIOC_AFRH |= (2 << ((8-8)*4));                    // PC8: в AFRH, сдвиг (8-8)*4=0

    TIM3_PSC = 89;
    TIM3_ARR = 999;


    TIM3_CCMR1 &= ~((7 << 4) | (7 << 12));
    TIM3_CCMR1 |= (6 << 4) | (1 << 3) | (6 << 12) | (1 << 11);

    TIM3_CCMR2 &= ~(7 << 4);
    TIM3_CCMR2 |= ((6 << 4) | (1 << 3));

    TIM3_CCER |= ((1 << 0) | (1 << 4) | (1 << 8));

    TIM3_CCR1 = 500;   // зелёный на ~50%
    TIM3_CCR2 = 250;   // синий на ~25%
    TIM3_CCR3 = 750;   // красный на ~75%

    TIM3_CR1 |= (1 << 0);

    // Основной цикл
    while (1) {

        for (uint16_t i=0; i<=999; i++) {
            TIM3_CCR1 = i;
            TIM3_CCR3 = 999 - i;
            TIM3_CCR2 = 0;
            delay(2000);
        }

        for (uint16_t i=0; i<=999; i++) {
            TIM3_CCR2 = i;
            TIM3_CCR1 = 999 - i;
            TIM3_CCR3 = 0;
            delay(2000);
        }

        for (uint16_t i=0; i<=999; i++) {
            TIM3_CCR3 = i;
            TIM3_CCR2 = 999 - i;
            TIM3_CCR1 = 0;
            delay(2000);
        }
    }
}