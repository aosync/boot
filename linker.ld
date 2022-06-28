OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_boot)

SECTIONS {
	. = 0x0;

	. = 0x500;
	. = ALIGN(4096);
	_p4 = .;
	. += 4096;
	_p3 = .;
	. += 4096;
	_p2_1 = .;
	. += 4096;
	_p2_2 = .;
	. += 4096;
	_workspace = .;

	. = 0x7c00;
	
	_stack_top = .;

	.boot : {
		KEEP (*(.boot.bootsec))
		*(boot.bootsec)

		_boot_stage1 = .;
		KEEP (*(.boot.stage1))
		*(koot.stage1)
	}
	.text : {
	      KEEP (*(.text))
	      *(.text.*)
	} :text
	.data : {
	      KEEP (*(.data))
	      *(.data.*)
	} :data
	.bss : {
	      KEEP (*(.bss))
	      *(.bss.*)
	} :data

	_boot_end = .;
}