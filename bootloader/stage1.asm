[bits 16]
[org 0x9000]

begin:
    mov bx, .msg
    call print
    call print_nl
    jmp $

    .msg: db "Stage1 loaded!", 0

%include "16bit_print.asm"