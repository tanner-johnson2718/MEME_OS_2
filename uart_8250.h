#ifndef MEME_UART_8250
#define MEME_UART_8250

// Standard 8250 UART SERIAL IO port and registers
#define COM1		  0x3F8
#define COM2          0x2F8
#define COM3          0x3E8
#define COM4          0x2E8

#define DEFAULT_COM               COM1
#define DEFAULT_COM_BAUD          115200
#define DEFAULT_COM_DATA_RDY_IRQ  1
#define DEFAULT_COM_DATA_SENT_IRQ 0
#define DEFAULT_COM_LSR_IRQ       0
#define DEFAULT_COM_MSR_IRQ       0
#define DEFAULT_COM_WORD_LEN      3      // 0,1,2,3
#define DEFAULT_COM_STOP_BIT      0      // 0,1
#define DEFAULT_COM_PARITY        7      // 0,1,3,5,7
#define DEFAULT_COM_BREAK_COND    0      // 0,1

// TODO: FIFO?, MCR?

#define COM_RX		  0x0	// In:	Receive buffer (DLAB=0)
#define COM_TX		  0x0	// Out: Transmit buffer (DLAB=0)
#define COM_DLL		  0x0	// Out: Divisor Latch Low (DLAB=1)
#define COM_DLH		  0x1	// Out: Divisor Latch High (DLAB=1)
#define COM_IER		  0x1	// Out: Interrupt Enable Register
#define COM_IER_RDI	  0x1	//   Enable receiver data interrupt
#define COM_IIR		  0x2	// In:	Interrupt ID Register
#define COM_FCR		  0x2	// Out: FIFO Control Register
#define COM_LCR		  0x3	// Out: Line Control Register
#define	COM_LCR_DLAB  0x80	//   Divisor latch access bit
#define	COM_LCR_WLEN8 0x03	//   Wordlength: 8 bits
#define COM_MCR		  0x4	// Out: Modem Control Register
#define	COM_MCR_RTS	  0x02	// RTS complement
#define	COM_MCR_DTR	  0x01	// DTR complement
#define	COM_MCR_OUT2  0x08	// Out2 complement
#define COM_LSR		  0x5	// In:	Line Status Register
#define COM_LSR_DATA  0x01	//   Data available
#define COM_LSR_TXRDY 0x20	//   Transmit buffer avail
#define COM_LSR_TSRE  0x40	//   Transmitter off
#define COM_MSR       0x6

#define MAX_BAUD      115200   // Maximum Buad rate i.e. Divisor latch = 0x1

void serial_init();
void serial_puts(char* s);
void serial_putc(char data);
void serial_putd(int d);
void serial_put_hex(int h);
char serial_getc();
int serial_get_buad();
void serial_set_buad(int r);

#endif