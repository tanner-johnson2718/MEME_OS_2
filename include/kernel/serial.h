#ifndef MEME_SERIAL
#define MEME_SERIAL

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

/* Bare bones UART 8250 driver. Assumes no FIFO on chip. Does very little
   besides set the BAUD rate and the predefined defaults.

  For more details see "docs/UART 8250 and 8259 PIC.htm".
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


/******************************************************************************
NAME)     serial_init_output

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_init_output();



/******************************************************************************
NAME)     serial_init_input

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_init_input();


/******************************************************************************
NAME)     serial_register_input_handler

INPUTS)   
          0) handler - Function pointer that takes in the inputed ascii char

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_register_input_handler(void (*handler)(u8));



/******************************************************************************
NAME)     serial_puts


INPUTS)   
          0) u8* s - c string to output 

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_puts(u8* s);



/******************************************************************************
NAME)     serial_putd


INPUTS)   
          0) u32 d- number to output in decimal

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_putd(u32 d);



/******************************************************************************
NAME)     serial_put_hex


INPUTS)   
          0) u32 g- number to output in hex

OUTPUTS)  NONE

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_put_hex(u32 h);



/******************************************************************************
NAME)     serial_getc


INPUTS)   NONE

OUTPUTS)  
          0) u8* c - pointer to one byte buffer to get inputed char

RETURNS)  0, always succeeds

COMMENTS) NONE
******************************************************************************/
u8 serial_getc(u8* c);

#endif