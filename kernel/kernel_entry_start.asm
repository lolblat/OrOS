
extern kernel_main
global start

section .text
[bits 32]
start:
    mov esp,0x90000
    mov ebp,esp
    push ebx
    call kernel_main
    hlt