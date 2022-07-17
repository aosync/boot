OUTPUT_FORMAT(elf64-x86-64)
ENTRY(boot)

SECTIONS {
	. = 0xffffffff80000000;

	.stack (NOLOAD) : {
		. = ALIGN(8);
		. += 0x10000;
		. = ALIGN(8);
		kernel_stack_high = .;
	} :stack


	kernel_begin = .;
	.text : {
		KEEP(*(.text .text.*))
	} :text

	. += CONSTANT(MAXPAGESIZE);

	.rodata : {
		KEEP(*(.rodata .rodata.*))
	} :rodata

	.data : {
		KEEP(*(.data .data.*))
	} :data

	.bss (NOLOAD) : {
		KEEP(*(COMMON))
	    KEEP(*(.bss .bss.*))
	} :data
	kernel_end = .;
}