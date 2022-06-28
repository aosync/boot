	[bits 16]

	extern stack_top

	section .boot.bootsec
	global _boot
_boot:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov gs, ax
	mov fs, ax

	mov esp, stack_top

	call drive_init

	call a20_enable_bios
	call a20_check
	cmp ax, 1
	je _boot.a20_ok

	call a20_enable_kb
	call a20_check
	cmp ax, 1
	je _boot.a20_ok

	call a20_enable_fast_gate
	call a20_check
	cmp ax, 1
	je _boot.a20_ok

_boot.a20_fail:
	hlt

_boot.a20_ok:
_boot.ld_stage1:

end:
	jmp $$

%include "bootsec.a20.S"
%include "bootsec.drive.S"
%include "bootsec.info.S"

%if ($ - $$) > 446
	%fatal "Bootstrap code is too large"
%endif


times 446 - ($ - $$) db 0
mbr.entry0:
mbr.entry0.attr: db 0x80
mbr.entry0.cs: 	 db 0x0
mbr.entry0.hs:   db 0x0
mbr.entry0.ss:   db 0x0
mbr.entry0.type: db 0x0
mbr.entry0.cl:   db 0x0
mbr.entry0.hl:   db 0x0
mbr.entry0.sl:   db 0x0
mbr.entry0.lb:   dd 1
mbr.entry0.len:  dd 512

times 510 - ($ - $$) db 0
db 0x55, 0xAA