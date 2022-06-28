; Initializes the bootdrive struct
drive_init:
	; Store the boot drive number
	mov byte [info.bootdrive.no], dl

	; Gather disk info
	mov ah, 0x8
	int 0x13

	; Store the number of heads
	; (the BIOS function returns the index of the last head)
	inc dh
	mov byte [info.bootdrive.heads], dh

	; Store the number of sectors per track
	and cl, 0x3f
	mov byte [info.bootdrive.spt], cl

	ret

; Converts esi to es:bx
drive_addrconv:
	push esi

	mov ax, si
	shr esi, 16
	mov dx, si

	mov si, 16
	div si

	mov es, ax
	mov bx, dx

	pop esi
	ret

; Reads the boot drive at LBA and writes it in esi
drive_read:
	push di 	; LBA to read
	push esi	; Destination

	call drive_addrconv

	mov ax, di
	mov cl, byte [info.bootdrive.spt]
	div cl

	mov cl, ah
	inc cl 		; Sector

	xor ah, ah
	mov dl, byte [info.bootdrive.heads]
	div dl

	mov dh, ah	; Head
	mov ch, al	; Cylinder

	mov ah, 0x2
	mov al, 1
	mov dl, byte [info.bootdrive.no]
	int 0x13

	pop esi
	pop di
	ret