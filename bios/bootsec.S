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
	jmp _boot.skip_ebpb
	times 3-($-$$) db 0x90

; The EBPB is a structure that may sometimes
; be required in order to properly boot
_boot.ebpb:
	; BPB
	bpb_oem_name: db "bsfs.fmt"
	bpb_sector:   dw 512
	bpb_cluster:  db 1
	bpb_rsvd_sec: dw 1
	bpb_fats:     db 2
	bpb_root_len: dw 224
	bpb_sectors:  dw 2880
	bpb_med_type: db 0xF0
	bpb_sec_fat:  dw 9
	bpb_spt:      dw 18
	bpb_heads:    dw 2
	bpb_hidden:   dd 0
	bpb_sec_huge: dd 0

	; FAT EBPB
	times 64 - ($ - $$) db 0

	; EBPB
	bpb_drive_no: db 0
	bpb_rsvd:     db 0
	bpb_sign:     db 0x29
	bpb_volid:    dd 0x2D7E5A1A
	bpb_vollbl:   db "BOOTLOADER!"
	bpb_fstype:   db "FAT32   "

%if ($ - $$) != 90
	%fatal "EBPB size is incorrect"
%endif

_boot.skip_ebpb:
	; Some BIOS set cs to 0x7c, reset it to zero
	jmp 0:_boot.segments
_boot.segments:
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
	call a20_check
	cmp ax, 1
	je _boot.a20_ok

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
	jmp $

_boot.a20_ok:
_boot.ld_stage1:
	mov edi, 1
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

%include "bootsec.info.S"
%include "bootsec.drive.S"
%include "bootsec.a20.S"

%if ($ - $$) > 446
	%fatal "Bootstrap code is too large"
%endif

times 510 - ($ - $$) db 0
db 0x55, 0xAA