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