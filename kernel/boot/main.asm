global start
extern kmain

section .text
bits 32
start:
	cli
	mov esp, stack_top
	mov ebp, esp

	call kmain
	hlt

section .bss
align 4096
stack_bottom:
	resb 4096 * 4
stack_top: