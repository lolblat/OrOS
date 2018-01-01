print_char:
	mov ah, 0x0e ; code to print char in TTY mode,
	int 0x10
	ret
	leave

print_string:
	;bx - address of string
	loop_until_null:
		mov al, [bx]
		or al,al
		jz end_of_loop
		call print_char
		inc bx	
		jmp loop_until_null

	end_of_loop:
		ret
		leave
