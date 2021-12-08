#include "uart_8250.h"

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

void serial_init()
{

}

int serial_get_buad()
{
    // read the ctl register, turn on the DLAB, write it back
    outb(DEFAULT_COM + COM_LCR, inb(DEFAULT_COM + COM_LCR) | 0x80);

    // get upper and lower DL value
    int ret = (inb(DEFAULT_COM + COM_DLH) << 8) + inb(DEFAULT_COM + COM_DLL);

    // Clear DLAB
    outb(DEFAULT_COM + COM_LCR, inb(DEFAULT_COM + COM_LCR) & 0x7f);

    return MAX_BAUD / ret;
}

void serial_set_buad(int rate)
{
    int dl = MAX_BAUD / rate;

    // read the ctl register, turn on the DLAB, write it back
    outb(DEFAULT_COM + COM_LCR, inb(DEFAULT_COM + COM_LCR) | 0x80);

    outb(DEFAULT_COM + COM_DLH, dl >> 8);
    outb(DEFAULT_COM + COM_DLL, dl & 0xff);

    // Clear DLAB
    outb(DEFAULT_COM + COM_LCR, inb(DEFAULT_COM + COM_LCR) & 0x7f);
}

void serial_putc(char data)
{
    outb(DEFAULT_COM, data);
}

void serial_putd(int d)
{
    int m = 10;    // log_10 (2^32) =~ 10
    char num[m];
    num[0] = 0;
    int i;
    for(i = 0; (i<m) && (d!=0); ++i)
    {
        num[i] = d % 10;
        d = d /10;
    }

    for(; i>=0; i--)
    {
        serial_putc(num[i] + '0');
    }
    
    
}

void serial_puts(char* s)
{
    int i = 0;
    while(s[i] != 0)
    {
        serial_putc(s[i]);
        ++i;
    }
}

char serial_getc()
{
    // wait till data rx line set high
    char rdy = 0;
    while(!rdy){
        rdy = inb((DEFAULT_COM + COM_LSR) & COM_LSR_DATA);
        serial_putc(rdy + 60);
    }

    return inb(DEFAULT_COM);

}