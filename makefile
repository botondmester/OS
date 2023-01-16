kernel_source_files := $(shell find kernel -name *.c)
kernel_header_files := $(shell find kernel -name *.h)
kernel_object_files := $(patsubst kernel/%.c, build/i386/%.o, $(kernel_source_files))

kernel_asm_source := $(shell find kernel -name *.asm)
kernel_asm_object := $(patsubst kernel/%.asm, build/i386/%.o, $(kernel_asm_source))

all: build-i386

$(kernel_object_files): build/i386/%.o : kernel/%.c
	mkdir -p $(dir $@) && \
	i386-elf-gcc -c -I kernel -ffreestanding $(patsubst build/i386/%.o, kernel/%.c, $@) -o $@

$(kernel_asm_object):  build/i386/%.o : kernel/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf32 $(patsubst build/i386/%.o, kernel/%.asm, $@) -o $@

.PHONY: build-i386
build-i386: $(kernel_asm_object) $(kernel_object_files)
	mkdir -p dist/i386 && \
	i386-elf-ld -n -o dist/i386/kernel.bin -T targets/i386/linker.ld $(kernel_asm_object) $(kernel_object_files) && \
	cp dist/i386/kernel.bin targets/i386/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/i386/kernel.iso targets/i386/iso