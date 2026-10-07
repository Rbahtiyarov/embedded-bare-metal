#include <stdint.h>

//2.2.2 Memory map and register boundary addresses
#define PERIPH_BASE         0x40000000UL
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000UL)
//База тактирования
#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800UL)
//Включение тактирования AHB1ENR
#define RCC_AHB1ENR         (*(volatile uint32_t *)(RCC_BASE + 0x30UL))

//Регистры порта C
#define GPIOC_BASE          (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOC_MODER         (*(volatile uint32_t *)(GPIOC_BASE + 0x00UL))
#define GPIOC_BSRR          (*(volatile uint32_t *)(GPIOC_BASE + 0x18UL))

#define RCC_AHB1ENR_GPIOCEN      (1UL << 2)
#define GPIOC_MODER_PC0_6_MASK   (0x3FFFUL)   // биты 13..0: поля MODER0..MODER6
#define GPIOC_MODER_PC0_6_OUT    (0x1555UL)   // в каждом поле 01 = output

#define SEG_PINS_MASK   (0x7FUL)   // PC0..PC6

#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof ((arr)[0]))

static void seg_write(uint8_t code)
{
    GPIOC_BSRR = ((~code & SEG_PINS_MASK) << 16) | code;
}

static void delay(volatile uint32_t count) {
    while (count--) { }
}

static const uint8_t seg_digits[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

int main(void) {
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC_MODER &= ~GPIOC_MODER_PC0_6_MASK;   // очистить 7 полей
    GPIOC_MODER |=  GPIOC_MODER_PC0_6_OUT;    // выставить output

    while(1) {
        for (uint8_t i=0; i<ARRAY_SIZE(seg_digits); i++) {
            seg_write(seg_digits[i]);
            delay(5000000);

        }

    }

}