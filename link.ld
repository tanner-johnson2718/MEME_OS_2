/* Linker map?? */

ENTRY(start)      /* symbol from entry.S?? */

SECTIONS {
    . = 0x200000;

    .text ALIGN (0x1000) :     /* Align to 4KB */
    {
        *(.text)               /* all text sections */
    }

    RODATA_START =.;         
    .rodata ALIGN (0x1000) :   /* Align to 4KB */
    {
        *(.rodata*)            /* all read only data sections */
    }

    DATA_START =.;
    .data ALIGN (0x1000) :     /* Align to 4KB */
    {
        *(.data)               /* all data sections */
    }

    BSS_START = .;
    .bss ALIGN (0x1000) :      /* Align to 4KB */
    {
        *(COMMON)              /* All COMMON sections */
        *(.bss)                /* all bss sections */
    }
}