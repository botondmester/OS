os.bin: bootloader/bootloader.bin #kernel.bin
	cat $< > os.bin

bootloader/bootloader.bin:
	cd bootloader && $(MAKE) clean all

run: os.bin
	qemu-system-i386 os.bin

clean:
	rm -f os.bin

all: clean run