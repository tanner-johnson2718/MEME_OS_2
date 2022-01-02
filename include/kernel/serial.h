#ifndef MEME_SERIAL
#define MEME_SERIAL

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

/* ..... FILL ME IN 


*/

///////////////////////////////////////////////////////////////////////////////
// Kernel Public Serial defines
///////////////////////////////////////////////////////////////////////////////

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



///////////////////////////////////////////////////////////////////////////////
// Kernel VGA API
///////////////////////////////////////////////////////////////////////////////

void serial_init();
void serial_puts(u8* s);
void serial_putd(u32 d);
void serial_put_hex(u32 h);
u32 serial_get_buad();
void serial_set_buad(u32 r);

#endif