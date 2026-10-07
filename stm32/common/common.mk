# Пути к общей обвязке (относительно папки проекта)
COMMON  = ../common

MCU     = -mcpu=cortex-m4 -mthumb
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

CFLAGS  = $(MCU) -Wall -O2 -ffreestanding -nostdlib -I$(COMMON)
LDFLAGS = -T $(COMMON)/linker.ld -nostdlib

# Исходники: локальный TARGET.c + общий startup.c
SRCS    = $(TARGET).c $(COMMON)/startup.c

all: $(TARGET).bin

$(TARGET).elf: $(SRCS) $(COMMON)/linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRCS) -o $@
	$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(TARGET).bin
	st-flash --reset write $(TARGET).bin 0x08000000

clean:
	rm -f *.elf *.bin