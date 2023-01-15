all: clean run

os.bin: bootloader/bootloader.bin #kernel.bin
	cat $^ > os.bin

bootloader/bootloader.bin:
	cd bootloader && $(MAKE) all

run: os.bin
	qemu-system-i386 -fda os.bin

clean:
	rm -f os.bin
	cd bootloader && $(MAKE) clean