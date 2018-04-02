
stage1_start:
    %include "stage1/bootsector.asm"
stage1_end:

stage2_start:
    %include "stage2/bootsector.asm"
    align 512,db 0
stage2_end:

kernel_start:
     incbin "../kernel/kernel.bin"
     align 512, db 0
kernel_end:
