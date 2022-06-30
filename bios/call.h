#ifndef BIOS_CALL_H
#define BIOS_CALL_H

#include <stdint.h>

extern struct {
	u32 eax;
	u32 ebx;
	u32 ecx;
	u32 edx;
	u32 edi;
	u32 esi;
	u16 es;
	u8 cf;
}__attribute__((packed)) bios_regs;

void bios_call(u8 int_no);

#endif