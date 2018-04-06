start_up_stage_2:
    mov ecx, STAGE_2_WELCOME
    call print_string

    ;enable A20 line
    in al,0x92
    or al,2
    out 0x92,al

    ;1) call memory detection and map the memory, and insert to booting info
    call memory_map
    mov dword [booting_info.memory_map_addr], ebx
    mov dword [booting_info.memory_map_count], ecx



    call load_kernel_to_memory

    ;3) protected mode time
    call switch_to_protected
    ;4) call the kernel and let the fun begin.
    jmp $



load_kernel_to_memory: ; 0x7e24 on bochs
    ;2) call unreal mod and load the kernel to high memory address
            ;2.1) load the kernel to tmp place
            ;2.2) call unreal mod
            ;2.3) load to high address
    push eax
    push ecx


    mov al,(kernel_end - kernel_start) / 512 ; first sector of kernel
    mov bx,stage2_end ;tmp buffer
    mov ch,0
    mov cl,(kernel_start-stage1_start)/ 512 + 1
    mov dh,0

    call load


    call unreal

    mov edi,KERNEL_ADDR
    mov esi,stage2_end

    mov ecx, 512 * 127
    cld
    a32 rep movsb
    pop ecx
    pop eax

    ret

[bits 32]
protected_mode:

    ;call load_kernel
    mov ebx, booting_info
    call KERNEL_ADDR
    hlt

booting_info:
    .memory_map_addr: dd 0
    .memory_map_count: dd 0

STAGE_2_WELCOME db "[D] Welcome to stage 2 of the boot :)",0
KERNEL_ADDR: equ 0x100000


%include "stage2/unreal_mod.asm"
%include "stage2/memory_detection.asm"
%include "stage2/protected_mod.asm"
%include "stage2/elf.asm"
