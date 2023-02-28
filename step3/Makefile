#.SUFFIXS : .c .o
#.SUFFIXS : .S .o
TARGET 	= main
#DEVICE 	= atmega2560
DEVICE 	= atmega128
#PORT	= /dev/tty.usbserial-0001
PORT	= /dev/ttyUSB0

CC = avr-gcc 
AS = avr-gcc
CFLAGS = -Wall -Os -mmcu=$(DEVICE) -I./inc
ASFLAGS = -Wall -Os -mmcu=$(DEVICE) -I./inc
LDFLAGS = -Wl,-T startup.lds -Wl,-nostdlib -nostartfiles -Wl,-Map=$(TARGET).map

CSRCS = $(wildcard ./*.c)
ASRCS = $(wildcard ./*.S)
SRCS = $(CSRCS) $(ASRCS)

COBJS = $(CSRCS:c=o)
AOBJS = $(ASRCS:S=o)
OBJS = $(COBJS) $(AOBJS)

# If you want to see the variables, use error, info
#$(error $(OBJS))

.PHONY: all convert upload clean

all: $(TARGET).elf convert 

$(TARGET).elf : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

convert: 
	avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	avr-size --format=avr --mcu=$(DEVICE) $(TARGET).elf
	avr-objdump -h -S -z $(TARGET).elf > $(TARGET).lss
	avr-objdump -D $(TARGET).elf > $(TARGET).dmp

	
upload:
	avrdude -v -p$(DEVICE) -cstk500 -P$(PORT) -U flash:w:$(TARGET).hex:i
clean:
	rm *.o 
	rm $(TARGET).elf
	rm $(TARGET).hex
	rm $(TARGET).map
	rm $(TARGET).lss
	rm $(TARGET).dmp
