# To assist in cross-compiling
CC=gcc
LDFLAGS=-pthread -L./lusb_build/lib -lusb-1.0
TOPDIR=$(shell pwd)

EEPROM_SUPPORT = y

BIGFILES=-D_FILE_OFFSET_BITS=64
CFLAGS=-O2 -std=gnu99 -static -Wall -I./lusb_build/include $(BIGFILES)

OBJS= flashcmd_api.o spi_controller.o spi_nand_flash.o spi_nor_flash.o ch341a_spi.o timer.o main.o

ifeq ($(EEPROM_SUPPORT),y)
CFLAGS += -DEEPROM_SUPPORT
OBJS += ch341a_i2c.o i2c_eeprom.o spi_eeprom.o
OBJS += bitbang_microwire.o mw_eeprom.o ch341a_gpio.o
endif

all: SNANDer

.lusb_install:
	mkdir -p lusb_build
	cd lusb_build; \
	../libusb-1.0.26/configure --prefix=$(TOPDIR)/lusb_build --disable-udev; \
	make; \
	make install
	touch $@

SNANDer: .lusb_install $(OBJS)
	$(CC) $(CFLAGS) -s -o $@ $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -f *.o SNANDer* .lusb_install*
	rm -rf lusb_build*
