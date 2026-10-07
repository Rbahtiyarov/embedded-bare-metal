# ==== Настройки проекта ====
TARGET  = main
SRCS    = blink.c startup.c

# ==== Тулчейн ARM ====
MCU     = -mcpu=cortex-m4 -mthumb
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

CFLAGS  = $(MCU) -Wall -O2 -ffreestanding -nostdlib
LDFLAGS = -T linker.ld -nostdlib

# ==== Правила сборки ====
all: $(TARGET).bin

$(TARGET).elf: $(SRCS) linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $@
	$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(TARGET).bin
	st-flash --reset write $(TARGET).bin 0x08000000

clean:
	rm -f *.elf *.bin
