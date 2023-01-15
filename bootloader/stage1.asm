[bits 16]
[org 0x9000]

begin:
    call cls
    mov bx, .msg
    call print
    call print_nl
    jmp $

    .msg: db "Stage1 loaded!", 0

cls:
    pusha
    mov ah, 0x00
    mov al, 0x03 ; text mode 80x25 16 colors
    int 0x10
    popa
    ret

%include "16bit_print.asm"

times 2048 - ($-$$) db 0