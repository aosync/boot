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

	; Store the nu
	and cl, 0x3f
	mov byte [info.bootdrive.spt], cl

	ret