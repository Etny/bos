
ENTRY(_start)

SECTIONS {
    . = 2M;
    
    .text BLOCK(4K) : ALIGN(4K) {
        . = ALIGN(8);
        KEEP(*(.multiboot))
        *(.text)
    }

    .boot BLOCK(4K) : ALIGN(4K) {
        *(.desc_tables)
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }

    . = ALIGN(4K);
    _end = .;
}
