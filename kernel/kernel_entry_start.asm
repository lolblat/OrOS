[bits 32]

KERNEL_VIRTUAL_ADDRESS_BASE equ 0xC0000000
KERNEL_PAGE_TABLE_ENTRY equ 769 - 1
PAGE_SIZE equ 0x1000
extern kernel_main
global _loader

section .data
align 0x1000
BootPageDir:
    times 1024 dd 0x2 ;not present, read write and kernel only

;%assign i 0
;%rep 769
;TableEntry_%+i:
 ;   times 1024 dd 0x3
;%assign i i + 1
;%endrep
TableEntry_0: ; for the first mb
    times 1024 dd 0x3

TableEntry_768: ; for the kernel
    times 1024 dd 0x3

section .text
align 4

loader equ (_loader - 0xC0000000)

STACK_SIZE equ 0x4000

_loader:
    push edx
    push ebx


    mov ecx,0
    .loop: ;4mb identity mapping
        cmp ecx, 1024
        jz .end
        mov eax,PAGE_SIZE
        imul eax,ecx
        or eax,3
        lea ebx, [TableEntry_0 - KERNEL_VIRTUAL_ADDRESS_BASE + 4*ecx]
        mov [ebx], eax
        inc ecx
        jmp .loop
    .end:

    mov ecx, 0

    .loop_768:
        cmp ecx,1024 ; 4 mega bytes of shit
        jz .end_768

        mov eax,PAGE_SIZE
        imul eax,ecx

        or eax, 0x3

        lea ebx,[TableEntry_768 - KERNEL_VIRTUAL_ADDRESS_BASE + 4 * ecx]
        mov [ebx], eax
        inc ecx
        jmp .loop_768
    .end_768:

    mov ecx,(BootPageDir - KERNEL_VIRTUAL_ADDRESS_BASE)
    mov ebx,ecx

    mov edx,(TableEntry_0 - KERNEL_VIRTUAL_ADDRESS_BASE )
    or edx,0x3
    mov dword[ebx],edx

    mov edx,(TableEntry_768 - KERNEL_VIRTUAL_ADDRESS_BASE) ; the address of the table
    or edx,0x3
    add ebx, KERNEL_PAGE_TABLE_ENTRY * 4 ; get the kernel pages
    mov dword[ebx], edx
    mov cr3, ecx ; base dir address


    mov ecx,cr0
    or ecx,0x80000000; enable paging
    mov cr0,ecx

    lea ecx,[HighHalfBitch] ; jmp to high half
    jmp ecx


extern kernel_virtual_end;
extern kernel_physical_end;

HighHalfBitch:
    pop ebx
    pop edx
    invlpg[0]


    mov esp,0xC0090000 + STACK_SIZE
    mov ebp,esp

    push kernel_virtual_end
    push kernel_physical_end
    push ebx

    call kernel_main
    hlt




