OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_boot)

SECTIONS {
	. = 0x0;

	. = 0x500;
	. = ALIGN(4096);
	bios_p4 = .;
	. += 4096;
	bios_p3 = .;
	. += 4096;
	bios_p2 = .;
	. += 4096;
	bios_p1 = .;
	. += 4096;
	bios_lower_workspace = .;

	. = 0x7c00;
	bios_stack_top = .;

	.boot : {
		KEEP(*(.boot.bootsec))

		bios_boot_stage1 = .;
		KEEP(*(.boot.stage1))
	}

	.text : {
		KEEP(*(.text .text.*))
	} :text

	.data : {
		KEEP(*(.data .data.*))
	} :data

	.bss (NOLOAD) : {
		KEEP(*(COMMON))
		KEEP(*(.bss .bss.*))
	} :data
	bios_boot_end = .;
	bios_upper_workspace = .;
}