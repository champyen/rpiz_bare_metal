.text

_start:
	ldr	sp, =stack_top
	bl	bare_metal_start

stack_top:      .word   0x100000
