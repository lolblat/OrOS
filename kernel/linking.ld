
ENTRY(loader)                /* the name of the entry label */

SECTIONS {
    . = 0xC0200000;          /* the code should be loaded at 2 MB */
  /* align at 4 KB and load at 1 MB */
    .text : AT(ADDR(.text)-0xC0000000)
    {
        *(.text)            /* all text sections from all files */
    }

    /* align at 4 KB and load at 1 MB + . */
    .data ALIGN (0x1000) : AT(ADDR(.data)-0xC0000000)
    {
        *(.data)            /* all data sections from all files */
    }

    /* align at 4 KB and load at 1 MB + . */
    .bss ALIGN (0x1000) : AT(ADDR(.bss)-0xC0000000)
    {
        *(COMMON)           /* all COMMON sections from all files */
        *(.bss)             /* all bss sections from all files */
    }
   kernel_virtual_end = .;
   kernel_physical_end = . - 0xC0000000;

}


