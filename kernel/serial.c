#include "serial.h"
#include "io_port.h"
#include "irq.h"
#include "log.h"

// See serial.h for "high level" documentation

///////////////////////////////////////////////////////////////////////////////
// Private Register and Macros
///////////////////////////////////////////////////////////////////////////////

#define SERIAL_COM_RX		  0x0	// In:	Receive buffer (DLAB=0)
#define SERIAL_COM_TX		  0x0	// Out: Transmit buffer (DLAB=0)
#define SERIAL_COM_DLL		  0x0	// Out: Divisor Latch Low (DLAB=1)
#define SERIAL_COM_DLH		  0x1	// Out: Divisor Latch High (DLAB=1)
#define SERIAL_COM_IER		  0x1	// Out: Interrupt Enable Register
#define SERIAL_COM_IER_RDI	  0x1	//   Enable receiver data interrupt
#define SERIAL_COM_IIR		  0x2	// In:	Interrupt ID Register
#define SERIAL_COM_FCR		  0x2	// Out: FIFO Control Register
#define SERIAL_COM_LCR		  0x3	// Out: Line Control Register
#define	SERIAL_COM_LCR_DLAB  0x80	//   Divisor latch access bit
#define	SERIAL_COM_LCR_WLEN8 0x03	//   Wordlength: 8 bits
#define SERIAL_COM_MCR		  0x4	// Out: Modem Control Register
#define	SERIAL_COM_MCR_RTS	  0x02	// RTS complement
#define	SERIAL_COM_MCR_DTR	  0x01	// DTR complement
#define	SERIAL_COM_MCR_OUT2  0x08	// Out2 complement
#define SERIAL_COM_LSR		  0x5	// In:	Line Status Register
#define SERIAL_COM_LSR_DATA  0x01	//   Data available
#define SERIAL_COM_LSR_TXRDY 0x20	//   Transmit buffer avail
#define SERIAL_COM_LSR_TSRE  0x40	//   Transmitter off
#define SERIAL_COM_MSR       0x6

#define SERIAL_MAX_BAUD      115200   // Maximum Buad rate i.e. Divisor latch = 0x1

// Standard 8250 UART SERIAL IO port and registers
#define SERIAL_COM1		     0x3F8
#define SERIAL_COM2          0x2F8
#define SERIAL_COM3          0x3E8
#define SERIAL_COM4          0x2E8

// registered input handler
void (*serial_internal_handler)(u8) = {0};

///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////

void serial_putc(u8 data)
{
    outb(SERIAL_DEFAULT_COM, data);
}

u32 serial_get_buad()
{
    // read the ctl register, turn on the DLAB, write it back
    outb(SERIAL_DEFAULT_COM + SERIAL_COM_LCR, inb(SERIAL_DEFAULT_COM + SERIAL_COM_LCR) | 0x80);

    // get upper and lower DL value
    u32 ret = (inb(SERIAL_DEFAULT_COM + SERIAL_COM_DLH) << 8) + inb(SERIAL_DEFAULT_COM + SERIAL_COM_DLL);

    // Clear DLAB
    outb(SERIAL_DEFAULT_COM + SERIAL_COM_LCR, inb(SERIAL_DEFAULT_COM + SERIAL_COM_LCR) & 0x7f);

    return SERIAL_MAX_BAUD / ret;
}

// Only called from within, assume rate good
void serial_set_buad(u32 rate)
{
    u32 dl = SERIAL_MAX_BAUD / rate;

    // read the ctl register, turn on the DLAB, write it back
    outb(SERIAL_DEFAULT_COM + SERIAL_COM_LCR, inb(SERIAL_DEFAULT_COM + SERIAL_COM_LCR) | 0x80);

    outb(SERIAL_DEFAULT_COM + SERIAL_COM_DLH, dl >> 8);
    outb(SERIAL_DEFAULT_COM + SERIAL_COM_DLL, dl & 0xff);

    // Clear DLAB
    outb(SERIAL_DEFAULT_COM + SERIAL_COM_LCR, inb(SERIAL_DEFAULT_COM + SERIAL_COM_LCR) & 0x7f);
}

void serial_irq()
{
    u8 in = inb(SERIAL_DEFAULT_COM);
    if(serial_internal_handler)
    {
        serial_internal_handler(in);
    }
    else
    {
        log_msg(__FILE__, __LINE__, "Serial IRQ generated w/ no internal handler registered");
    }
}

///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////


/******************************************************************************
NAME)     serial_init_output

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_init_output()
{

    // Set defualt baud rate set in configs
    serial_set_buad(SERIAL_DEFAULT_COM_BAUD);
    log_val(__FILE__, __LINE__, "8250 Serial Baud", SERIAL_DEFAULT_COM_BAUD);

    // Enable inturrepts we want to handle
    u8 ier = inb(SERIAL_DEFAULT_COM + SERIAL_COM_IER);
    ier = ier | (SERIAL_DEFAULT_COM_DATA_RDY_IRQ);
    ier = ier | (SERIAL_DEFAULT_COM_DATA_SENT_IRQ << 1);
    ier = ier | (SERIAL_DEFAULT_COM_LSR_IRQ << 2);
    ier = ier | (SERIAL_DEFAULT_COM_MSR_IRQ << 3);
    outb(SERIAL_DEFAULT_COM + SERIAL_COM_IER, ier);
    log_val(__FILE__, __LINE__, "8250 Serial IER", ier);

    // Set LCR values
    u8 lcr = 0;
    lcr = lcr | SERIAL_DEFAULT_COM_WORD_LEN;
    lcr = lcr | (SERIAL_DEFAULT_COM_STOP_BIT << 2);
    lcr = lcr | (SERIAL_DEFAULT_COM_PARITY << 3);
    lcr = lcr | (SERIAL_DEFAULT_COM_BREAK_COND << 6);
    outb(SERIAL_DEFAULT_COM + SERIAL_COM_LCR, lcr) ;
    log_val(__FILE__, __LINE__, "8250 Serial LCR", lcr);
    

    return 0;
}



/******************************************************************************
NAME)     serial_init_input

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_init_input()
{
    // register input irq handler
    if(SERIAL_DEFAULT_COM ==  SERIAL_COM1 || SERIAL_DEFAULT_COM == SERIAL_COM3)
    {
        if(irq_register_PIC_handler(serial_irq, IRQ_PIC_COM2))
        {
            log_msg(__FILE__, __LINE__, "serial failed to register IRQ handler");
        }
    }
    else
    {
        if(irq_register_PIC_handler(serial_irq, IRQ_PIC_COM1))
        {
            log_msg(__FILE__, __LINE__, "serial failed to register IRQ handler");
        }
    }

    log_msg(__FILE__, __LINE__, "Serial input init-ed");
    return 0;
}


/******************************************************************************
NAME)     serial_register_input_handler

INPUTS)   
          0) handler - Function pointer that takes in the inputed ascii char

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_register_input_handler(void (*handler)(u8))
{
    serial_internal_handler = handler;
    return 0;
}



/******************************************************************************
NAME)     serial_putd


INPUTS)   
          0) u32 d- number to output in decimal

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_putd(u32 d)
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
    
    return 0;
}



/******************************************************************************
NAME)     serial_put_hex


INPUTS)   
          0) u32 g- number to output in hex

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_put_hex(u32 d)
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

    return 0;
}



/******************************************************************************
NAME)     serial_puts


INPUTS)   
          0) u8* s - c string to output 

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_puts(u8* s)
{
    u32 i = 0;
    while(s[i] != 0)
    {
        serial_putc(s[i]);
        ++i;
    }

    return 0;
}


/******************************************************************************
NAME)     serial_getc


INPUTS)   NONE

OUTPUTS)  
          0) u8* c - pointer to one byte buffer to get inputed char

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_getc(u8* c)
{
    *c = inb(SERIAL_DEFAULT_COM);
    return 0;
}