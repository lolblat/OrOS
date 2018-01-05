[org 0x7c00]
[bits 16]
KERNEL_OFFSET equ 0x1000
start:
	mov [BOOT_DRIVE],dl
	; start with printing in 16 bit cpu mode
	xor ax,ax
	mov ds,ax
	mov es,ax
	mov sp,0x9000
	mov bp, sp
	mov bx, HELLO_16_BIT
	call print_string

	mov ah,0
	mov al,3
	int 0x10; change screen mode to print in protected mode


	;load kernel section
	call load_kernel
	
	call switch_to_protected
loop_real_mode:
	jmp loop_real_mode


[bits 16]
load_kernel:
	mov bx, KERNEL_OFFSET
	mov dh,15
	mov dl, [BOOT_DRIVE]
	call load_real_mode
	ret
	leave
%include "print/print_real_mode.asm"
%include "switch_to_protected.asm"
%include "gdt.asm"
%include "print/print_protected_mode.asm"
%include "load_real_mode.asm"
[bits 32]
protected_mode:
	mov ax, 0x10
	mov ds,ax
	mov ss,ax
	mov es,ax
	mov fs,ax
	mov esp,9000h
	mov ebx, HELLO_32_BIT
	call print_string_32_bit	
	call KERNEL_OFFSET
protected_mode_loop:
	jmp protected_mode_loop

BOOT_DRIVE db 0
HELLO_16_BIT db "Hello from 16 bit CPU System",13,10,0
HELLO_32_BIT db "Hello from 32 bit CPU System",0
times 510 - ($ - $$) db 0
dw 0xaa55
