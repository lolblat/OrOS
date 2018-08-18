;code switch to protected mode
[bits 16]
switch_to_protected:
	cli ; disable int
	lgdt[gdt_start.size] ; load the idt into the memory

	mov eax, cr0
	or al, 1 ; switch flag true
	mov cr0, eax

	jmp 08h:protected_mod_init ; far jump to code

	ret


[bits 32]
protected_mod_init:
        mov ax,gdt_start.gdt_data_descriptor
        mov ds,ax
        mov es,ax
        mov ss,ax
        mov fs,ax
        mov gs,ax
        mov esp,0x7c00
        mov ebp,esp
        call protected_mode

gdt_start:
.gdt_data:
	dd 0x0 ; 8 bytes of null descriptor
	dd 0x0
	;-----------------------------;
.gdt_code_descriptor: equ $ - gdt_start
	dw 0xFFFF ; limit low
	dw 0x0 ;base low
	db 0x0 ; base middle
	db 10011010b ; access
	db 11001111b ; granularity
	db 0x0
.gdt_data_descriptor: equ $ - gdt_start
	dw 0xFFFF ; limit low
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0
.gdt_user_code: equ $ - gdt_start
 dw 0xFFFF
 dw 0x0
 db 0x0
 db 11111010b
 db 11001111b
 db 0x0
.gdt_user_data: equ $ - gdt_start
 dw 0xFFFF
 dw 0x0
 db 0x0
 db 11110010b
 db 11001111b
 db 0x0
.end_of_gdt:
.size:
	dw .end_of_gdt - .gdt_data - 1
	dd .gdt_data
