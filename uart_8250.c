#include "uart_8250.h"
#include "io_port.h"

void serial_init()
{
    // Set defualt baud rate set in configs
    serial_set_buad(DEFAULT_COM_BAUD);

    // Enable inturrepts we want to handle
    u8 ier = inb(DEFAULT_COM + COM_IER);
    ier = ier | (DEFAULT_COM_DATA_RDY_IRQ);
    ier = ier | (DEFAULT_COM_DATA_SENT_IRQ << 1);
    ier = ier | (DEFAULT_COM_LSR_IRQ << 2);
    ier = ier | (DEFAULT_COM_MSR_IRQ << 3);
    outb(DEFAULT_COM + COM_IER, ier);

    // Set LCR values
    u8 lcr = 0;
    lcr = lcr | DEFAULT_COM_WORD_LEN;
    lcr = lcr | (DEFAULT_COM_STOP_BIT << 2);
    lcr = lcr | (DEFAULT_COM_PARITY << 3);
    lcr = lcr | (DEFAULT_COM_BREAK_COND << 6);
    outb(DEFAULT_COM + COM_LCR, lcr) ;

    // Set FIFO ??

    // Set MCR ??

    // Put device info over serial
    serial_puts("UART 8250 Device found)");
    serial_puts("\n\r   BAUD : ");
    serial_put_hex(serial_get_buad());
    serial_puts("\n\r   IER  : ");
    serial_put_hex(inb(DEFAULT_COM + COM_IER));
    serial_puts("\n\r   IIR  : ");
    serial_put_hex(inb(DEFAULT_COM + COM_IIR));
    serial_puts("\n\r   LCR  : ");
    serial_put_hex(inb(DEFAULT_COM + COM_LCR));
    serial_puts("\n\r   MCR  : ");
    serial_put_hex(inb(DEFAULT_COM + COM_MCR));
    serial_puts("\n\r   LSR  : ");
    serial_put_hex(inb(DEFAULT_COM + COM_LSR));
    serial_puts("\n\r   MSR  : ");
    serial_put_hex(inb(DEFAULT_COM + COM_MSR));
}

u32 serial_get_buad()
{
    // read the ctl register, turn on the DLAB, write it back
    outb(DEFAULT_COM + COM_LCR, inb(DEFAULT_COM + COM_LCR) | 0x80);

    // get upper and lower DL value
    u32 ret = (inb(DEFAULT_COM + COM_DLH) << 8) + inb(DEFAULT_COM + COM_DLL);

    // Clear DLAB
    outb(DEFAULT_COM + COM_LCR, inb(DEFAULT_COM + COM_LCR) & 0x7f);

    return MAX_BAUD / ret;
}

void serial_set_buad(u32 rate)
{
    u32 dl = MAX_BAUD / rate;

    // read the ctl register, turn on the DLAB, write it back
    outb(DEFAULT_COM + COM_LCR, inb(DEFAULT_COM + COM_LCR) | 0x80);

    outb(DEFAULT_COM + COM_DLH, dl >> 8);
    outb(DEFAULT_COM + COM_DLL, dl & 0xff);

    // Clear DLAB
    outb(DEFAULT_COM + COM_LCR, inb(DEFAULT_COM + COM_LCR) & 0x7f);
}

void serial_putc(u8 data)
{
    outb(DEFAULT_COM, data);
}

void serial_putd(u32 d)
{
    u32 m = 10;
    u8 num[m];
    num[0] = 0;
    u32 i;
    for(i = 0; (i<m); ++i)
    {
        num[i] = d % 10;
        d = d / 10;

        if(d == 0)
        {
            break;
        }
    }

    while(1)
    {
        serial_putc(num[i] + '0');

        if(i == 0)
        {
            break;
        }
        i--;
    }
    
    
}

void serial_put_hex(u32 d)
{
    u32 m = 8;
    u8 num[m];
    num[0] = 0;
    u32 i;
    for(i = 0; (i<m); ++i)
    {
        num[i] = d % 16;
        d = d / 16;

        if(d == 0)
        {
            break;
        }
    }

    serial_putc('0');
    serial_putc('x');
    while(1)
    {
        if(num[i] <= 9)
        {
            serial_putc(num[i] + '0');
        }
        else
        {
           serial_putc(num[i] - 10 + 'A'); 
        }

        if(i == 0)
        {
            break;
        }
        --i;
    }
}

void serial_puts(u8* s)
{
    u32 i = 0;
    while(s[i] != 0)
    {
        serial_putc(s[i]);
        ++i;
    }
}

u8 serial_getc()
{
    // wait till data rx line set high
    u8 rdy = 0;
    while(!rdy){
        rdy = inb((DEFAULT_COM + COM_LSR) & COM_LSR_DATA);
        serial_putc(rdy + 60);
    }

    return inb(DEFAULT_COM);

}