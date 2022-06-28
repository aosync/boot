[bits 16]

section .boot.stage1

global _stage1
_stage1:
	mov ah, 0x0E
	mov al, 'H'
	mov bh, 0x00
	int 10h
	jmp $