[bits 16]
[org 0x7C00]

begin:
    mov bp, 0x8000
    mov sp, bp

    mov bx, 0x9000
    mov dh, 0x000f
    call disk_read

print:
    pusha
    .loop:
        mov al, [bx]
        cmp al, 0
        je .done

        mov ah, 0x0E
        int 0x10

        add bx, 1
        jmp .loop
    .done:
        popa
        ret

print_nl:
    pusha

    mov ah, 0x0E
    mov al, 0x0A
    int 0x10
    mov al, 0x0D
    int 0x10

    popa
    ret

print_hex:
    pusha
    mov cx, 0

    .loop:
        cmp cx, 4
        je .done

        mov ax, dx
        and ax, 0x000f # mask ax
        add ax, 0x30 # convert N to ascii "N"
        cmp ax, 0x39 # if it's > 9 then add 7 to represent 'A' to 'F'
        jle step2
        add ax, 7
    .step2:
        mov bx, .HEX_OUT + 5 # put the character in the right place
        sub bx, cx
        mov [bx], al
        ror dx, 4

        add cx, 1
        jmp .loop
    .done:
        mov bx, .HEX_OUT
        call print

        popa
        ret
    .HEX_OUT:
        db '0x0000',0

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