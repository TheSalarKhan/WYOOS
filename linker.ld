ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
    . = 0x00100000;
    .text ALIGN(0x1000) :
    {
        code = .; _code = .; __code = .;
        *(.multiboot)
        *(.text)
    }

    .data :
    {
        data = .; _data = .; __data = .;
	start_ctors = .;
	KEEP(*( .init_array ));
	KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
	end_ctors = .;
        *(.data)
        *(.rodata)
    }

    .bss :
    {
        bss = .; _bss = .; __bss = .;
        *(.bss)
    }

    /DISCARD/ :
    {
        *(.fini_array*)
	*(.comment)
    }
    end = .; _end = .; __end = .;
}
