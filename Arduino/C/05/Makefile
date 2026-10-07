MCU     = atmega328p
F_CPU   = 16000000UL
PORT    = /dev/arduino_uno
TARGET  = seven_elem_display

CC      = avr-gcc
CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall

all: $(TARGET).hex

$(TARGET).elf: $(TARGET).c
	$(CC) $(CFLAGS) $< -o $@

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	avrdude -c arduino -p $(MCU) -P $(PORT) -b 115200 -U flash:w:$<:i

clean:
	rm -f *.o *.elf *.hex
