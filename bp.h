#ifndef BP_H
#define BP_H

#include <stdint.h>

enum bp_res_code {
	BP_RES_ABORT = 0x0,
	BP_RES_RELOCATE_STACK = 0x1,
};

struct bp_bootreq {
	u64 next;
	u64 stack_low;
	u64 stack_high;
}__attribute__((packed));
typedef struct bp_bootreq BpBootreq;

struct bp_bootinfo {
	u64 phoff;
	u64 stack_low;
	u64 stack_high;
}__attribute__((packed));
typedef struct bp_bootinfo BpBootinfo;

#endif