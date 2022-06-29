#ifndef STDINT_H
#define STDINT_H

#include <assert.h>

// Normal types
typedef unsigned char uchar;

typedef unsigned short int ushort;

typedef unsigned int uint;

typedef unsigned long int ulong;

typedef long long int vlong;
typedef unsigned long long int uvlong;

// Bit granular types available on this machine
typedef unsigned char u8;
typedef char i8;
// Assumption that char is 8 bits.

typedef unsigned short int u16;
typedef short int i16;
static_assert(sizeof(u16) == 2, "u16 is not 16-bits");
static_assert(sizeof(i16) == 2, "i16 is not 16-bits");

typedef unsigned int u32;
typedef int i32;
static_assert(sizeof(u32) == 4, "u32 is not 32-bits");
static_assert(sizeof(i32) == 4, "i32 is not 32-bits");

typedef unsigned long long int u64;
typedef long long int i64;
static_assert(sizeof(u64) == 8, "u64 is not 64-bits");
static_assert(sizeof(i64) == 8, "i64 is not 64-bits");

typedef unsigned long long int size_t;
static_assert(sizeof(size_t) == 8, "size_t is not 64-bits");

#endif