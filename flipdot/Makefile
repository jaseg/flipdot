
all: main.elf


main.elf: main.c
	avr-gcc -mmcu=atmega2560 -O2 -Wall -Wpedantic -Wstrict-aliasing -o $@ $<

.PHONY: program
program: main.elf
	avrdude -V -c usbtiny -P usb -p atmega2560 -U flash:w:main.elf

.PHONY: clean
clean:
	rm -f main.elf
