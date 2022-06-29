#ifndef STDINT_H
#define STDINT_H

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

typedef unsigned short int u16;
typedef short int i16;

typedef unsigned int u32;
typedef int i32;

typedef unsigned long long int u64;
typedef long long int i64;

typedef unsigned long long int size_t;

#endif