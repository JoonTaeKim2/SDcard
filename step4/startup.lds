OUTPUT_FORMAT("elf32-avr","elf32-avr","elf32-avr")
OUTPUT_ARCH(avr:5)
ENTRY(_start)

MEMORY
{
  flash   (rx)   : org = 0,        len = 64K
  sram    (rw!x) : org = 0x800100, len = 4K
}

SECTIONS
{
    .text :
    {
        *(.vectors)
        . = 0x0046 ;
        *(.text.ASM)
        *(.text)
        *(.text.*)
        . = ALIGN(2);
    } > flash

    .data :
    {
        __data_start = .;
        __cli_start = .;
        KEEP(*(SORT(.cli.*)))
        __cli_end = .;
        *(.data)
        *(.data*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(2);
        __data_end = .;
    } > sram AT > flash
    __data_load_start = LOADADDR(.data);

    .bss :
    {
        __bss_start = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    } > sram

    . = ALIGN(2);
    __heap_start = .;
    __heap_end = ORIGIN(sram)+LENGTH(sram)-1;
}
