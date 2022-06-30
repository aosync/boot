[bits 16]

; Linker definitions
extern bios_stack_top
extern bios_boot_stage1
extern bios_boot_end

; stage1
extern _stage1

section .boot.bootsec
global _boot
_boot:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov gs, ax
	mov fs, ax

	; Initialize the stack
	mov esp, bios_stack_top

	sidt [bios_info.bioslidtr]

	call drive_init

	; Attempt to enable the A20 line
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
	mov esi, str.a20_error
	call fatal

_boot.a20_ok:
_boot.ld_stage1:
	mov edi, dword [mbr.entry0.lb]
	mov esi, bios_boot_stage1
_boot.ld_stage1.loop0b:
	cmp esi, bios_boot_end
	jge _boot.ld_stage1.loop0e

	call drive_read

	inc edi
	add esi, 0x200

	jmp _boot.ld_stage1.loop0b
_boot.ld_stage1.loop0e:
	jmp _stage1

%include "bootsec.a20.S"
%include "bootsec.drive.S"
%include "bootsec.log.S"
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