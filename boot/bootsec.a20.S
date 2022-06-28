; Attempts to enable the A20 line with the BIOS function
a20_enable_bios:
	mov ax, 0x2401
	int 0x15
	ret

; Attempts to enable the a20 line using the keyboard
; controller
a20_enable_kb:
	pushf
	cli

	; Disable PS/2 port
	call kb_wait_ready
	mov al, 0xAD
	out 0x64, al

	; Request to send port byte
	call kb_wait_ready
	mov al, 0xD0
	out 0x64, al

	; Receive output port byte
	call kb_wait_avail
	in al, 0x60
	push eax

	; Request to receive new port byte
	call kb_wait_ready
	mov al, 0xd1
	out 0x64, al

	; Send new port byte
	call kb_wait_ready
	pop eax
	; enable bit 1 (A20 line)
	or al, 2
	out 0x60, al

	; Reenable the PS/2 port
	call kb_wait_ready
	mov al, 0xAE
	out 0x64, al

	call kb_wait_ready

	popf
	ret

; Waits for the keyboard controller to be ready to receive
kb_wait_ready:
	in al, 0x64
	; Bit 1 indicates that the input buffer is full
	test al, 2
	jnz kb_wait_ready
	ret

; Waits for the keyboard controller to have sent data
kb_wait_avail:
	in al, 0x64
	; Bit 0 indicates that the output buffer is pending
	test al, 1
	jz kb_wait_avail
	ret

; Attempts to enable the A20 gate using the Fast Gate Method
a20_enable_fast_gate:
	in al, 0x92
	or al, 2
	out 0x92, al
	ret

; 16bits check whether the A20 line is enabled
; ax := status (1 OK, 0 disabled)
a20_check:
	pushf
	push ds
	push es
	push di
	push si

	cli

	; Put first segment in es
	xor ax, ax
	mov es, ax

	; Put last segment in ds
	not ax
	mov ds, ax

	; Put addresses that should correspond to each
	; other if the A20 line is disabled.
	; The 0x10 in si would make the address rollover to
	; 0x500
	mov di, 0x0500
	mov si, 0x0510

	; Save the previous bytes at the test locations
	; (normally, there should be nothing of note there)
	mov ax, word [es:di]
	push ax

	mov ax, word [ds:si]
	push ax

	; Place different bytes at each locations
	mov word [es:di], 0x0
	mov word [ds:si], 0x1

	; Check if placing a byte at [ds:si] has overriden
	; the byte at [es:di]
	cmp word [es:di], 0x1

	; Restore test location bytes
	pop ax
	mov word [ds:si], ax

	pop ax
	mov word [es:di], ax

	; Return accordingly
	mov ax, 0
	je a20_check.end
	mov ax, 1

a20_check.end:
	pop si
	pop di
	pop es
	pop ds
	popf
	ret