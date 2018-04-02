;file that represent unreal mod
[bits 16]
unreal:
    cli ; disable all int's

    push ds
    push es

    lgdt[ptr_to_gdt_of_unreal] ; load the gdt entry

    mov eax,cr0 ; start pmode
    or al, 1
    mov cr0, eax

    jmp $+2
    ;mov bx, gdt_unreal_start.data
    mov bx, gdt_unreal_start.data
    mov ds,bx
    mov es,bx

    and al,0xFE ; back to real mode
    mov cr0,eax ; toggle cr0 bit
    pop es
    pop ds
    sti
    ret
ptr_to_gdt_of_unreal:
    dw gdt_unreal_start.gdt_unreal_end - gdt_unreal_start - 1
    dd gdt_unreal_start

gdt_unreal_start:
.null_pad: dq 0
.data: equ $ - gdt_unreal_start
    ;Flags Code(0), ExpandDown(0), Writeable(1), Acessed(0)
    dw 0xffff ; Limit(bits 0 - 15)
    dw 0; Base(bits 0 - 15)
    db 0; Base(16 - 23)
    db 10010010b; First flags, type flags
    db 11001111b; second flags, Limit(bits 16 - 19)
    db 0; Base(bits 24 - 31)
.gdt_unreal_end: