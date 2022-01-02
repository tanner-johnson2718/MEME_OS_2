#ifndef MEME_IRQ
#define MEME_IRQ

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

// The IRQ module is the MEME OS 2 interrupt handler. This module first
// handles allocating and gives public functions for manipulating the IDT. The
// IDT associates an interrupt with a handler. All interrupts will be handled
// at least initially in kernel space i.e the IDT entry will execute in the 
// kernel code sector. All interrupts will be treated as interrupt as opposed
// to a trap.

// The second main function is to act as a PIC 8259 driver. Module will init
// this device, and remap its interrupts to 32-47. Publuc API functions are
// exposed so drivers may handle IRQs generated by PIC devices. Sending EOI
// and other hous keeping are handled by the IRQ module, as driver registered
// handlers are stored as function pointers and called by the base handler.

// Finally the IRQ module adds functionality for defining handlers for soft
// interrupts, as well as kernel public functions for raising soft interrupts.
// 0x30 - 0x3F will be reserved for such interrupts. Again IRQ base handlers
// handle the house keeping (like managing the stack pointer, etc.).

// CPU generated exceptions are reserved for 0x0 - 0x1F and as of now are not
// handled and no functions are exported in an API to register handlers. In the
// future the irq module will implement the proper handlers for this excpetions

// For more details see "docs/8259 PIC - OSDEV Wiki.hmtl", "docs/BKD_IDT.htm",
// "docs/BKD_IRQ_PIC.htm", "docs/UART 8250 and 8259 PIC.htm".

///////////////////////////////////////////////////////////////////////////////
// Kernel Public IRQ defines
///////////////////////////////////////////////////////////////////////////////

// PIC Interupts
#define IRQ_PIC_MASTER_BASE 0x20
#define IRQ_PIC_SLAVE_BASE  0x28

#define IRQ_PIC_TIMER       IRQ_PIC_MASTER_BASE + 0x0
#define IRQ_PIC_KEY         IRQ_PIC_MASTER_BASE + 0x1
#define IRQ_PIC_SLAVE       IRQ_PIC_MASTER_BASE + 0x2
#define IRQ_PIC_COM1        IRQ_PIC_MASTER_BASE + 0x3
#define IRQ_PIC_COM2        IRQ_PIC_MASTER_BASE + 0x4
#define IRQ_PIC_SOUND       IRQ_PIC_MASTER_BASE + 0x5
#define IRQ_PIC_FLOPPY      IRQ_PIC_MASTER_BASE + 0x6
#define IRQ_PIC_LPT1        IRQ_PIC_MASTER_BASE + 0x7

#define IRQ_PIC_RT_CLOCK    IRQ_PIC_SLAVE_BASE + 0x0
#define IRQ_PIC_IRQ2        IRQ_PIC_SLAVE_BASE + 0x1
#define IRQ_PIC_PCI1        IRQ_PIC_SLAVE_BASE + 0x2
#define IRQ_PIC_PCI2        IRQ_PIC_SLAVE_BASE + 0x3
#define IRQ_PIC_MOUSE       IRQ_PIC_SLAVE_BASE + 0x4
#define IRQ_PIC_MATH        IRQ_PIC_SLAVE_BASE + 0x5
#define IRQ_PIC_HD          IRQ_PIC_SLAVE_BASE + 0x6
#define IRQ_PIC_RESV        IRQ_PIC_SLAVE_BASE + 0x7

// Soft Interrupts (unused for now)
#define IRQ_SOFT_0 0x30
#define IRQ_SOFT_1 0x31
#define IRQ_SOFT_2 0x32
#define IRQ_SOFT_3 0x33
#define IRQ_SOFT_4 0x34
#define IRQ_SOFT_5 0x35
#define IRQ_SOFT_6 0x36
#define IRQ_SOFT_7 0x37
#define IRQ_SOFT_8 0x38
#define IRQ_SOFT_9 0x39
#define IRQ_SOFT_A 0x3A
#define IRQ_SOFT_B 0x3B
#define IRQ_SOFT_C 0x3C
#define IRQ_SOFT_D 0x3D
#define IRQ_SOFT_E 0x3E
#define IRQ_SOFT_F 0x3F

// Error Codes
#define IRQ_INVALID_ENTRY 1
#define IRQ_RAISER_NOT_IMPLEMENTED 2

///////////////////////////////////////////////////////////////////////////////
// Kernel Public IRQ defines
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     irq_init

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always succeds

COMMENTS) NONE
******************************************************************************/
u8 irq_init();



/******************************************************************************
NAME)     irq_register_PIC_handler

INPUTS)   
          0) handler - Function pointer with no input or returns to handle
                       indicated PIC interrupt. The devices that use the PIC
                       are defined above.
          1) entry   - The entry specifying which interrupt your handler is
                       handling. Must be between 0x20 and 0x2F.

OUTPUTS)  NONE

RETURNS)  0 on success or else non zero error code on failure

COMMENTS) NONE
******************************************************************************/
u8 irq_register_PIC_handler(void (*handler)(void), u8 entry);



/******************************************************************************
NAME)     irq_register_soft

INPUTS)   
          0) handler - Function pointer with no input or returns to handle
                       indicated soft interrupt.
          1) entry   - The entry specifying which interrupt your handler is
                       handling. Must be between 0x30 and 0x3F.

OUTPUTS)  NONE

RETURNS)  0 on success or else non zero error code on failure

COMMENTS) NONE
******************************************************************************/
u8 irq_register_soft(void (*handler)(void), u8 entry);



/******************************************************************************
NAME)     irq_raise

INPUTS)   
          0) entry   - The entry specifying which interrupt your are raising.
                       Must be between 0x30 and 0x3F.

OUTPUTS)  NONE

RETURNS)  0 on success or else non zero error code on failure

COMMENTS) NONE
******************************************************************************/
u8 irq_raise(u8  entry);


/******************************************************************************
NAME)     irq_on

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always succeds

COMMENTS) turn on interrupts
******************************************************************************/
u8 irq_on();



/******************************************************************************
NAME)     irq_off

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0, always succeds

COMMENTS) turn off interrupts
******************************************************************************/
u8 irq_off();

#endif