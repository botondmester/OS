all: stage0.bin stage1.bin
	cat $< > bootloader.bin

%.bin: %.asm
	nasm -f bin $< -o $@

clean:
	rm *.bin