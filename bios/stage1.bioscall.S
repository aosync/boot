extern bios_info.bioslidtr

global bios_regs
bios_regs:
bios_regs.eax: dd 0
bios_regs.ebx: dd 0
bios_regs.ecx: dd 0
bios_regs.edx: dd 0
bios_regs.edi: dd 0
bios_regs.esi: dd 0
bios_regs.es:  dw 0
bios_regs.cf:  db 0

bios_call.prev_idtr:
	dq 0
	dw 0
bios_call.prev_gdtr:
	dq 0
	dw 0

; Calls a particular interrupt from long mode using the
; given interrupt number and register state
global bios_call
bios_call:
	pushf

	; Save the code segment
	mov ax, cs
	push ax
	push ax

	; Save the data segment
	mov ax, ds
	push ax

	; Save the interrupt number
	push di

	sidt [bios_call.prev_idtr]
	sgdt [bios_call.prev_gdtr]

	lgdt [gdtr32]
	push (gdt32.code - gdt32)
	push bios_call.pmdown
	retfq
[bits 32]
bios_call.pmdown:
	; Disable paging
	mov eax, cr0
	and eax, ~(1 << 31)
	mov cr0, eax

	; Disable long mode
	mov ecx, 0xC0000080
	rdmsr
	and eax, ~(1 << 8)
	wrmsr

	lgdt [gdtr16]
	jmp (gdt16.code - gdt16):bios_call.pmdown2
[bits 16]
bios_call.pmdown2:
	; Here we are in 16 bits protected mode

	; Disable protected mode
	mov eax, cr0
	and eax, ~1
	mov cr0, eax

	jmp 0:bios_call.realdown
bios_call.realdown:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov gs, ax
	mov fs, ax

	lidt [bios_info.bioslidtr]

	; Modify the interrupt number
	pop ax
	mov byte [bios_call.int + 1], al

	; Load the registers
	mov eax, dword [bios_regs.eax]
	mov ebx, dword [bios_regs.ebx]
	mov ecx, dword [bios_regs.ecx]
	mov edx, dword [bios_regs.edx]
	mov edi, dword [bios_regs.edi]
	mov esi, dword [bios_regs.esi]
	mov es, word [bios_regs.es]
	mov byte [bios_regs.cf], 0

	sti

bios_call.int:
	int 0x0

	cli

	jnc bios_call.carry_set
	mov byte [bios_regs.cf], 1
bios_call.carry_set:
	mov word [bios_regs.es], es
	mov dword [bios_regs.esi], esi
	mov dword [bios_regs.edi], edi
	mov dword [bios_regs.edx], edx
	mov dword [bios_regs.ecx], ecx
	mov dword [bios_regs.ebx], ebx
	mov dword [bios_regs.eax], eax

	mov eax, cr0
	or eax, 1
	mov cr0, eax

	lgdt [gdtr32]
	jmp (gdt32.code - gdt32):bios_call.pmup
[bits 32]
bios_call.pmup:
	; Restore long mode
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; Restore paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	; Restore the long mode ds
	pop ax

	; At this point, cs is already (2 times) in the stack
	; We can proceed with a long jump
	push bios_call.lmup
	lgdt [bios_call.prev_gdtr]
	retf
[bits 64]
bios_call.lmup:
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov es, ax
	mov ds, ax

	; XXX
	; lidt [bios_call.prev_idtr]
bios_call.end:
	popf
	ret
