// Entry point for the kernel

extern int kernel_start;
extern int kernel_end;

#define COM1            0x3F8   // 
#define COM_LSR		    5	    // In: Line Status Register
#define COM_LSR_DATA	0x01	//     Data available
#define COM_LSR_TXRDY	0x20	//     Transmit buffer avail
#define COM_LSR_TSRE	0x40	//     Transmitter off

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

void serial_putc(char data)
{
    outb(COM1, data);
}

char serial_getc()
{
    // wait till data rx line set high
    char rdy = 0;
    while(!rdy){
        rdy = inb((COM1 + COM_LSR) & COM_LSR_DATA);
        serial_putc(rdy + 60);
    }

    return inb(COM1);

}

void kernel_main(void) 
{
    for(;;){
        char c = serial_getc();
        serial_putc(c);
    }
}