#ifndef MEME_SERIAL
#define MEME_SERIAL

#include "core/types.h"

// Standard 8250 UART SERIAL IO port and registers
#define SERIAL_COM1		  0x3F8
#define SERIAL_COM2          0x2F8
#define SERIAL_COM3          0x3E8
#define SERIAL_COM4          0x2E8

#define SERIAL_DEFAULT_COM               SERIAL_COM1
#define SERIAL_DEFAULT_COM_BAUD          115200
#define SERIAL_DEFAULT_COM_DATA_RDY_IRQ  1
#define SERIAL_DEFAULT_COM_DATA_SENT_IRQ 0
#define SERIAL_DEFAULT_COM_LSR_IRQ       0
#define SERIAL_DEFAULT_COM_MSR_IRQ       0
#define SERIAL_DEFAULT_COM_WORD_LEN      3      // 0,1,2,3
#define SERIAL_DEFAULT_COM_STOP_BIT      0      // 0,1
#define SERIAL_DEFAULT_COM_PARITY        7      // 0,1,3,5,7
#define SERIAL_DEFAULT_COM_BREAK_COND    0      // 0,1

// TODO: FIFO?, MCR?

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

void serial_init();
void serial_puts(u8* s);
void serial_putc(u8 data);
void serial_putd(u32 d);
void serial_put_hex(u32 h);
u32 serial_get_buad();
void serial_set_buad(u32 r);

#endif