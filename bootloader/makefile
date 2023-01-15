all: clean bootloader.bin

bootloader.bin: stage0.bin stage1.bin
	cat $^ > bootloader.bin

%.bin: %.asm
	nasm -f bin $< -o $@

clean:
	rm -f *.bin