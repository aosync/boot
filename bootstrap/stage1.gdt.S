gdt16:
gdt16.nil:
	dq 0
gdt16.data:
	dw 0xFFFF
	dw 0
	db 0
	db 0b10010010
	db 0b10001111
	db 0
gdt16.code:
	dw 0xFFFF
	dw 0
	db 0
	db 0b10011010
	db 0b10001111
	db 0
gdtr16:
	dw (gdtr16 - gdt16)
	dd gdt16

gdt32:
gdt32.nil:
	dq 0
gdt32.data:
	dw 0xFFFF
	dw 0
	db 0
	db 0b10010010
	db 0b11001111
	db 0
gdt32.code:
	dw 0xFFFF
	dw 0
	db 0
	db 0b10011010
	db 0b11001111
	db 0
gdtr32:
	dw (gdtr32 - gdt32)
	dd gdt32

gdt64:
gdt64.nil:
	dq 0
gdt64.data:
	dw 0xFFFF
	dw 0
	db 0
	db 0b10010010
	db 0b11001111
	db 0
gdt64.code:
	dw 0xFFFF
	dw 0
	db 0
	db 0b10011010
	db 0b10101111
	db 0
gdtr64:
	dw (gdtr64 - gdt64)
	dq gdt64