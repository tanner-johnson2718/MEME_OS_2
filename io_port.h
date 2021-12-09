#ifndef MEME_IO_PORT
#define MEME_IO_PORT

#include "types.h"

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

#endif