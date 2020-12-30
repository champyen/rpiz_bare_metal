OUTPUT_ARCH(arm)
SECTIONS {
	. = 0x8000;

	.text . : {
		*(.text)
	}
	. = ALIGN(4);
	.rodata . : {
		*(.rodata)
		*(.rodata.*)
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
	. = 0x8000 + 0x1fff;
	.tail : {
		*(.tail)
	}
}
