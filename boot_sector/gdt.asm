gdt_start:
gdt_data:
	dd 0x0 ; 8 bytes of null descriptor
	dd 0x0
	;-----------------------------;
gdt_code_descriptor:
	dw 0xFFFF ; limit low
	dw 0x0 ;base low
	db 0x0 ; base middle
	db 10011010b ; access
	db 11001111b ; granularity
	db 0x0
gdt_data_descriptor:
	dw 0xFFFF ; limit low
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0

end_of_gdt:
size:
	dw end_of_gdt - gdt_data - 1
	dd gdt_data
