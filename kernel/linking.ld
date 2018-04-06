
ENTRY(loader)                /* the name of the entry label */

SECTIONS {
    . = 0xC0100000;          /* the code should be loaded at 1 MB */

    .text : AT(ADDR(.text) - 0xC0000000) {
        *(.text)             /* all text sections from all files */
        *(.rodata*)
    }

   .data ALIGN (0x1000) : AT(ADDR(.data) - 0xC0000000) {
          *(.data)
      }

      .bss : AT(ADDR(.bss) - 0xC0000000) {
          _sbss = .;
          *(COMMON)
          *(.bss)
          _ebss = .;
      }
}


