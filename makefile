os.bin: bootloader/bootloader.bin #kernel.bin
	cat $^ > os.bin

bootloader/bootloader.bin:
	cd bootloader && $(MAKE)

run: os.bin
	qemu-system-i386 -fda os.bin

clean:
	rm -f os.bin

all: clean run