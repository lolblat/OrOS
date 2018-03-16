;Find free chunks of memory with e820.
;Draw the memory map of the os.
;As we know with the memory Map article from os dev. https://wiki.osdev.org/Memory_Map_(x86)
;We have several free RAM spaces, that we can use to store our table in, one space is in 0x0500
;ES:DI destenation buffer.
;edx - magic number 0x534D4150
;eax - e820
;ecx - 24(number of bytes to read)
;do int 15h

;the call to int, will change ebx, when ebx will be equal zero, it's mean that the list of free memory is over.
;so we wont reset ebx every call, we need to change ecx = 24, eax = e820 and inc di

[bits 16]
memory_map:
    .start equ 0x0600
    .end equ 0x5000
    .count equ 0x0596
    xor ebx,ebx
    mov edx,.start
    mov es,edx
    mov di, 0
    mov esi, 0
    call .lpcall
    ret
    leave

.lpcall:
    inc esi
    mov edx,0x534D4150
    mov eax,0xe820
    mov ecx,24
    int 0x15
    test ebx,ebx
    jz .over
    add di, 24
    jmp .lpcall

.over:
    mov [.count], si
    ret
    leave