; Logs a string but halts
fatal:
	call log
	jmp $

; Logs a string
log:
	mov ah, 0xE
	mov bh, 0x0
	mov bl, 0xC

log.loop0b:
	lodsb
	cmp al, 0x0
	je log.loop0e

	int 10h

	jmp log.loop0b
log.loop0e:

	mov ah, 0x3
	int 0x10

	inc dh
	mov dl, 0

	mov ah, 0x2
	int 0x10

	ret

str.a20_error:
	db "A20 error", 0