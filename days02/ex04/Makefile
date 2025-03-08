all: 
	make kill hex flash screen

kill:
	killall screen

screen:
	screen /dev/ttyUSB0 115200

hex:
	avr-gcc -Os -D F_CPU=16000000UL -mmcu=atmega328p -o  main.bin *.c && avr-objcopy -O ihex main.bin main.hex

flash:
	avrdude -p m328p -c arduino -b 115200 -P /dev/ttyUSB0 -U flash:w:main.hex

clean :
	 rm *.hex && rm *.bin