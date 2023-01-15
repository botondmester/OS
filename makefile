kernel_source_files := $(shell find kernel -name *.c)
kernel_header_files := $(shell find kernel -name *.h)
kernel_object_files := $(patsubst kernel/%.c, build/x86_64/%.o, $(kernel_source_files))

kernel_asm_source := $(shell find kernel -name *.asm)
kernel_asm_object := $(patsubst kernel/%.asm, build/x86_64/%.o, $(kernel_asm_source))

all: build-x86_64

$(kernel_object_files): build/x86_64/%.o : kernel/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc -c -I kernel -ffreestanding $(patsubst build/x86_64/%.o, kernel/%.c, $@) -o $@

$(kernel_asm_object):  build/x86_64/%.o : kernel/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst build/x86_64/%.o, kernel/%.asm, $@) -o $@

.PHONY: build-x86_64
build-x86_64: $(kernel_asm_object) $(kernel_object_files)
	mkdir -p dist/x86_64 && \
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld $(kernel_asm_object) $(kernel_object_files) && \
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso