os.bin: bootloader/bootloader.bin #kernel.bin
	cat $< > os.bin

run: os.bin
	qemu-system-i386 os.bin

clean:
	rm os.bin

all: clean run