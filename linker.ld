OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_boot)

SECTIONS {
	. = 0x0;

	. = 0x7c00;
	
	stack_top = .;

	.boot : {
		KEEP (*(.boot.bootsec))
		*(boot.bootsec)
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

	boot_end = .;
}