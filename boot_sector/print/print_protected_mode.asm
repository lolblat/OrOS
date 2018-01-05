[bits 32]
VGA_ADD equ 0xb8000
WHITE_ON_BLACK equ 0x0f
print_char_32_bit:
	;edx - index at memory
	;al - char

	mov ah, WHITE_ON_BLACK
	mov [edx] , ax
	ret
	leave

print_string_32_bit:
	;ebx - address of string
	pusha
	mov edx,VGA_ADD
	loop_string_32_bit:
		mov al, [ebx] ; load char into al
		or al, al ; check zero
		jz end_printing_32_bit
		call print_char_32_bit
		inc ebx	
		add edx , 2	
		jmp loop_string_32_bit
	end_printing_32_bit:
		popa
		ret
		leave
