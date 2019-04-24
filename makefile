# *** Set MCU here:
MCU = attiny44

CC = avr-gcc
TARGET = main.c

all: 
	$(CC) $(TARGET) -mmcu=$(MCU) -DCONFIG_$(MCU)=1 -O2 -o firmware
	mv firmware bin/firmware.elf
	avr-objcopy -O ihex bin/firmware.elf bin/firmware.hex

flash:
	sudo avrdude -c avrispmkII -P /dev/ttyACM0 -p t44 -u -U flash:w:bin/firmware.hex

nightly: all
