[org 0x7c00]
start:
	; start with printing in 16 bit cpu mode
	mov sp,0x9000
	mov bp, sp
	mov bx, HELLO_16_BIT
	call print_string
	mov ah,0
	mov al,3
	int 0x10

	call switch_to_protected
loop_real_mode:
	jmp loop_real_mode

%include "print_real_mode.asm"
%include "switch_to_protected.asm"
%include "gdt.asm"
%include "print_protected_mode.asm"

[bits 32]
protected_mode:
	mov ax, 0x10
	mov ds,ax
	mov ss,ax
	mov es,ax
	mov esp,9000h
	mov ebx, HELLO_32_BIT
	call print_string_32_bit	
	jmp $

HELLO_16_BIT db "Hello from 16 bit CPU System",13,10,0
HELLO_32_BIT db "Hello from 32 bit CPU System",0
times 510 - ($ - $$) db 0
dw 0xaa55
