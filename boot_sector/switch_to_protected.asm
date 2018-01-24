;code switch to protected mode
switch_to_protected:
	cli ; disable int
	lgdt[size] ; load the idt into the memory

	mov eax, cr0
	or al, 1 ; switch flag true
	mov cr0, eax

	jmp 08h:protected_mode ; far jump to code
	ret
	leave
