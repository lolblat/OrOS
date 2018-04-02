[org 0x7c00]
[bits 16]

boot:
    xor ax,ax
    mov fs,ax
    mov ds,ax
    mov ss,ax
    mov gs,ax
    mov es,ax

    jmp 0x0:.init ;set code segment by far jump
.init:
    mov sp, 0x7c00 ; set stack to 0x7c00
    mov [BOOT_FROM_DISK],dl ; save the disk started
    ;1) load stage 2 of the bootsector
    mov cx,STAGE_1_STARTED
    call print_string
    call print_new_line

    mov bx,stage2_start
    mov al, (stage2_start - stage1_start) / 512 ;512 - size of secto
    mov ch,0
    mov cl,2
    mov dh,0
    call load

    jmp start_up_stage_2


    jmp $
    ;2) jmp to stage2

;use chs addressing system.
;al - total sector count max 72
;ch - cylinder & 0xff
;cl - sector|((cylinder >> 2)& 0xC0)
;dh - head
;es:bx - read to buffer
;dl - drive number
load:
    mov ah,2
    mov dl,[BOOT_FROM_DISK]

    int 0x13
    jc error

    ret


error: ;error in all bootsector
    mov cx, ERROR_PRINT
    call print_string
    hlt

%include "stage1/print.asm"

ERROR_PRINT db "[E] Error in program",0
STAGE_1_STARTED db "[D] Stage 1 started :)",0
BOOT_FROM_DISK equ 0x0
times 510-($-$$) db 0
dw 0xaa55