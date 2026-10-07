/* Память STM32F446RE */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);   /* вершина стека — конец RAM */

SECTIONS
{
    /* Код и таблица векторов -> во flash */
    .text : {
        KEEP(*(.isr_vector))    /* таблица векторов в самом начале */
        *(.text*)               /* весь код */
        *(.rodata*)             /* константы */
        _etext = .;
    } > FLASH

    _sidata = LOADADDR(.data);

    /* Инициализированные данные -> живут в RAM, копия хранится во flash */
    .data : {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > RAM AT > FLASH

    /* Неинициализированные данные -> в RAM, обнуляются стартап-кодом */
    .bss : {
        _sbss = .;
        *(.bss*)
        _ebss = .;
    } > RAM
}