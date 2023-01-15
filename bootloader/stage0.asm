[bits 16]
[org 0x7C00]

begin:
    mov bp, 0x8000
    mov sp, bp

    mov bx, 0x9000
    mov dh, 0x0002 # load 2 sectors for stage 2
    call disk_read
    jmp 0x9000

%include "16bit_print.asm"

disk_read:
    # args:
    #   dl: drive number
    #   dh: sectors to read
    pusha
    
    push dx

    mov ah, 0x02 # read
    mov al, dh   # sectors to read
    mov cl, 0x02 # read from sector 2
    mov ch, 0x00 # cylinder 0
    mov dh, 0x00 # head 0
    # dl is set by caller

    int 0x13 # int 13h
    jc .read_error

    pop dx

    cmp al, dh
    jne .sector_error

    popa
    ret

    .disk_error:
        mov bx, .DISK_ERROR
        call print
        call print_nl
        mov dh, ah
        call print_hex
        jmp .panic
    .sector_error:
        mov bx, .SECTORS_ERROR
        call print
        jmp .panic
    .panic:
        jmp $
    
    .DISK_ERROR: db "Disk read error", 0
    .SECTORS_ERROR: db "Incorrect number of sectors read", 0

times 510 - ($-$$) db 0
dw 0xaa55