;load from hard drive
load_real_mode:
	mov ah,2; assert that ah equals 2
	mov al,dh
	mov ch,0
	mov cl,2
	mov dh,0
	int 0x13; calls int 0x13 bios int, read al sectors to es:bx add
	jc error_loading_real_mode
	ret
	leave
error_loading_real_mode:
	mov bx, LOAD_ERROR
	call print_string
	hlt

LOAD_ERROR db "Error while loading from disk",0,10

	
