[bits 64]

; void __attribute__((cdecl)) GDT_Load(GDTDescriptor* descriptor, uint16_t codeSegment, uint16_t dataSegment);
global GDT_Load
GDT_Load:

    ; make new call frame
    push rbp
    mov rbp, rsp

    ; load gdt
    mov rax, [rbp + 8]
    lgdt [rax]

    ; reload code segment
    mov rax, [rbp + 12]
    push rax
    push .reload_cs
    retf

    .reload_cs:
        ; reload data segments
        mov ax, [rbp + 16]
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        ; restore old call frame
        mov rsp, rbp
        pop rbp
        ret