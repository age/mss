MCU = atmega324p
CC = avr-gcc $(CFLAGS) $(INCLUDES) $(DEFS)
OBJCOPY = avr-objcopy -j .text -j .data -O ihex

CFLAGS = -Os -mmcu=$(MCU) -Wl,-u,vfprintf -lprintf_flt -lm -std=c99 -Wall -pedantic
INCLUDES = -Iusbdrv/ -I. -Ilib302dl/
FILES = crc8.c ds18x20.c humidity.c lcd.c main.c onewire.c temperature.c \
usbdrv/oddebug.c usbdrv/usbdrv.c usbdrv/usbdrvasm.S twimaster.c lib302dl/lib302dl.c
DEFS = -DF_CPU=20000000UL -DOW_ONE_BUS -DLIS_SDO_HIGH
AVRDUDE = avrdude

all: clean main.hex

main.hex: main.o main.elf main

main: $(main.elf)
	@echo OBJCOPY $@.elf $@.hex
	@$(OBJCOPY) $@.elf $@.hex

main.o: $(FILES)
	@echo CC $(FILES) -c
	@$(CC) $(FILES) -c

main.elf: $(main)
	@echo "CC $@"
	@$(CC) -o $@ *.o

asm: $(FILES)
	@$(CC) $(FILES) -S

clean:
	@rm -f *.o main.elf main.hex
flash: all
	@sudo $(AVRDUDE) -c usbasp -p m324p -U flash:w:main.hex
