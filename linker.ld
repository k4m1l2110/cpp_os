ENTRY(loader)
OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH("i386:x86-64")

SECTIONS
{
    . = 0x100000;

    .text : {
        *(.multiboot)
        *(.text*)
        *(.rodata)
    }

    .data : {
        start_ctors = .;
        KEEP(*(.init_anyway));
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)));
        end_ctors = .;
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    /DISCARD/ : {
        *(.fini_array*)
        *(.comment)
    }
}
