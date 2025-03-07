all: 
	make hex &&  make flash

hex:
	avr-gcc -Os -D F_CPU=16000000UL -mmcu=atmega328p -o  main.bin main.c && avr-objcopy -O ihex main.bin main.hex

flash:
	avrdude -p m328p -c arduino -b 115200 -P /dev/ttyUSB0 -U flash:w:main.hex

clean :
	 rm *.hex && rm *.bin