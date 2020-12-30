OUTPUT_ARCH(arm)
SECTIONS {
	. = 0x8000;

	.text . : {
		*(.text)
	}
	. = ALIGN(4);
	.data . : {
		*(.data)
	}
	. = ALIGN(4);
	.bss . : {
		*(.bss)
		*(COMMON)
	}
	. = ALIGN(4);
	.rodata . : {
		*(.rodata)
		*(.rodata.*)
	}
}
