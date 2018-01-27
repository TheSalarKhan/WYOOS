ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
    . = 0x00100000;
    .multiboot ALIGN(0x1000) :
    {
        *(.multiboot)
    }
    .text  :
    {
        code = .; _code = .; __code = .;
        *(.text)
    }

    .data :
    {
        data = .; _data = .; __data = .;
	start_ctors = .;
	*(.ctors.*)
	*(.ctors)
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
        *(.eh_frame)
        *(.fini_array*)
	    *(.comment)
    }
    end = .; _end = .; __end = .;
}
