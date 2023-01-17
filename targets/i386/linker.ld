ENTRY(start)

SECTIONS
{
	. = 1M;

	.boot :
	{
		KEEP(*(.multiboot_header))
	}

	.text :
	{
		kernel_text = .;
		*(.text)
		. = ALIGN(4096);
	}

	.data :
	{
		kernel_data = .;
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}

	.bss :
	{
		kernel_bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

	kernel_end = .;
}