#ifndef MEME_IO_PORT
#define MEME_IO_PORT

#include "types.h"

// Code to simply send bytes out the IO ports on the x86 CPU

static inline void outb(u32 port, u8 data)
{
	asm volatile("outb %0,%w1" : : "a" (data), "d" (port));
}

static inline u8 inb(u32 port)
{
	u8 data;
	asm volatile("inb %w1,%0" : "=a" (data) : "d" (port));
	return data;
}

static inline void outl(u32 port, u32 data)
{
	asm volatile("outl %0,%w1" : : "a" (data), "d" (port));
}

static inline u32 inl(u32 port)
{
	u32 data;
	asm volatile("inl %w1,%0" : "=a" (data) : "d" (port));
	return data;
}

#endif