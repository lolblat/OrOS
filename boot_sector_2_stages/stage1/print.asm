[bits 16]

;al - char to print
printchar:
    mov ah, 0x0e; print char option
    int 0x10 ; int to print
    ret

print_new_line:
    push ax
    mov al,13;\r
    call printchar

    mov al,10;\n
    call printchar
    pop ax
    ret

;cx - str
print_string:
    push bx
    mov bx,cx
    .string_loop:
        mov al,byte [bx]
        or al,al
        jz .end
        call printchar
        inc bx
        jmp .string_loop
    .end:
        pop bx
        ret
