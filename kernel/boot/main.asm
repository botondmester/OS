global start

section .text
bits 32
start:
	; print OK
	mov dword [0xb8000], 0x2f4f2f4b
	hlt
