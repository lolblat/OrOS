[org 0x7c00]
start:
	; start with printing in 16 bit mode
	mov sp,0x9000
	mov bp, sp
	mov bx, HELLO_16_BIT
	call print_string
loop_real_mode:
	jmp loop_real_mode

%include "print_real_mode.asm"

HELLO_16_BIT db "Hello from 16 bit CPU System",13,10,0
times 510 - ($ - $$) db 0
dw 0xaa55
