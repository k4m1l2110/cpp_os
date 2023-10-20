ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
.=0x0100000

.text   :  {
*(.multiboot)
*(.text*)
*(.rodata)
}

.data   :  {
    start_ctors = .;
    KEEP(*(.init_anyway));
    KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.*)));
    emd_ctors=.;

    *(.data)
}

.bss    :   {
    *(.bss)
}
}