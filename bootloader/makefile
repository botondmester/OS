all: stage1.bin stage0.bin
	cat $^ > bootloader.bin

%.bin: %.asm
	nasm -f bin $< -o $@

clean:
	rm -f *.bin