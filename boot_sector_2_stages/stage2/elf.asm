[bits 32]
;ebx - offset to elf
load_kernel:
    mov esi, [KERNEL_ADDR+0x1C] ; program header offset
    mov ebx, esi
    add ebx, KERNEL_ADDR

    mov ecx, [ebx+0x10] ; p_filesz- Size in bytes of the segment in the file image.
    mov edi, [ebx+0x08] ; p_vaddr- Virtual address of the segment in memory.
    mov esi, [ebx+0x04] ; p_offset- Offset of the segment in the file image.
    add esi, KERNEL_ADDR

    cld
    rep movsb

    mov eax, [KERNEL_ADDR+0x18] ; Entry point

    mov esi, [KERNEL_ADDR+0x1C]
    add esi, KERNEL_ADDR

    mov ebx, [esi+0x08] ; p_vaddr
    mov ecx, [esi+0x14] ; p_memsz- size of segment in memory
    ret
