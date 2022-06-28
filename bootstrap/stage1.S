%include "stage1.bioscall.S"
[bits 16]

; Pages
extern _p4
extern _p3
extern _p2
extern _p1

; C main function
extern main

%include "stage1.gdt.S"
; %include "stage1.bioscall.S"

section .boot.stage1

global _stage1
_stage1:
	mov eax, cr0
	or ax, 1
	mov cr0, eax

	lgdt [gdtr32]

	mov ax, (gdt32.data - gdt32)
	jmp (gdt32.code - gdt32):_stage1.pm
[bits 32]
_stage1.pm:
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov gs, ax
	mov fs, ax

_stage1.pages_clear:
	mov edi, _p4
	call page_clear
	mov edi, _p3
	call page_clear
	mov edi, _p2
	call page_clear
	mov edi, _p1
	call page_clear

_stage1.pages_init:
	mov edi, _p3
	or edi, 3
	mov [_p4], edi

	mov edi, _p2
	or edi, 3
	mov [_p3], edi

	mov edi, _p1
	or edi, 3
	mov [_p3 + 8], edi

	mov ebx, 1 << 7 | 0x3
	mov ecx, 512
	mov edi, _p2
_stage1.p2_loop:
	mov [edi], ebx
	add ebx, 0x200000
	add edi, 8
	loop _stage1.p2_loop

	mov ecx, 512
	mov edi, _p1
_stage1.p1_loop:
	mov [edi], ebx
	add ebx, 0x200000
	add edi, 8
	loop _stage1.p1_loop

	; Load the p4 into cr3
	mov edi, _p4
	mov cr3, edi

_stage1.pae_enable:
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax
_stage1.lm_enable:
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

_stage1.paging_enable:
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	lgdt [gdtr64]

	mov ax, gdt64.data - gdt64
	jmp (gdt64.code - gdt64):_stage1.lm

[bits 64]
_stage1.lm:
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov gs, ax
	mov fs, ax

	call main

	; mov dword [bios_regs.eax], 0xE << 8 | 'H'
	; mov dword [bios_regs.ebx], 0 << 8 | 0xD
	; mov edi, 0x10
	; call bios_call

	jmp $

; Clears a page
page_clear:
	mov ecx, 0
page_clear.loop0b:
	cmp ecx, 1024
	jge page_clear.loop0e

	mov dword [edi + 4*ecx], 0x0

	inc ecx
	jmp page_clear.loop0b
page_clear.loop0e:
	ret