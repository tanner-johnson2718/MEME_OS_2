#ifndef MEME_IO_PORT
#define MEME_IO_PORT

static inline void outb(int port, char data)
{
	asm volatile("outb %0,%w1" : : "a" (data), "d" (port));
}

static inline char inb(int port)
{
	char data;
	asm volatile("inb %w1,%0" : "=a" (data) : "d" (port));
	return data;
}

#endif