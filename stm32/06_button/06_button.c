#include <stdint.h>

#define PERIPH_BASE         0x40000000UL
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000UL)
//База тактирования
#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800UL)
//Включение тактирования AHB1ENR
#define RCC_AHB1ENR         (*(volatile uint32_t *)(RCC_BASE + 0x30UL))

//PA5 как выход — уже умеем (Blink).
//PC13 как вход — вот это новое: регистр IDR

//GPIOA
#define GPIOA_BASE          (AHB1PERIPH_BASE)
#define GPIOA_MODER         (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_IDR           (*(volatile uint32_t *)(GPIOA_BASE + 0x10UL))
#define GPIOA_PUPDR         (*(volatile uint32_t *)(GPIOA_BASE + 0x0CUL))
//GPIOC
#define GPIOC_BASE          (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOC_MODER         (*(volatile uint32_t *)(GPIOC_BASE + 0x00UL))
#define GPIOC_IDR           (*(volatile uint32_t *)(GPIOC_BASE + 0x10UL))
#define GPIOC_PUPDR         (*(volatile uint32_t *)(GPIOC_BASE + 0x0CUL))

#define GPIOA_MODER_PA5_MASK   (3UL << 10)
#define GPIOA_MODER_PA5_OUTPUT (1UL << 10)

#define GPIOC_MODER_PC13_MASK  (4UL << 26)

#define GPIOC_IDR_PC13         (1UL << 13)

// Включение тактирования GPIOA и GPIOC
#define RCC_AHB1ENR_GPIOAEN  (1UL << 0)
#define RCC_AHB1ENR_GPIOCEN  (1UL << 2)


int main(void) {
    RCC_AHB1ENR |= (RCC_AHB1ENR_GPIOAEN) | (RCC_AHB1ENR_GPIOCEN);

    GPIOA_MODER &= ~GPIOA_MODER_PA5_MASK;   // очистить биты 10-11
    GPIOA_MODER |=  GPIOA_MODER_PA5_OUTPUT;   // настроить биты 10-11 на  OUTPUT

    GPIOC_MODER &= ~GPIOC_MODER_PC13_MASK;  // очистить биты 26

    while (condition)
    {
        /* code */
    }
    
}