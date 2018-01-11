
.extern kernel_main
.global start

.section .bss
    .align 16
    stack_bottom:
        .skip 4096
    stack_top:

.section .text
    start:
        mov $stack_top, %esp
        call kernel_main
        hang:
            cli
            hlt
            jmp hang