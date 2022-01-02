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
// this device, remap its interrupts to 32-47. TODO FINSISH ME!!!!!!!!!!!!!!!

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


///////////////////////////////////////////////////////////
// IDT entry flag  bit fields
///////////////////////////////////////////////////////////

// flags byte [7] - Present bit. 
#define IRQ_IDT_ENTRY_PRESENT_SHIFT 0x7
#define IRQ_IDT_ENTRY_PRESENT       0x1
#define IRQ_IDT_ENTRY_NOT_PRESENT   0x0

// flags byte [5-6] - Privelege 
#define IRQ_IDT_ENTRY_PRIVILEGE            0x5
#define IRQ_IDT_ENTRY_KERNEL_PRIVLEGE      0x0
#define IRQ_IDT_ENTRY_USER_PRIVLEGE        0x3

// flags byte [4] - Always 0

// flags byte [0-3] - Gate Type
#define IRQ_IDT_ENTRY_TYPE_SHIFT 0x0
#define IRQ_IDT_ENTRY_TASK       0x5
#define IRQ_IDT_ENTRY_INTERRUPT  0xE
#define IRQ_IDT_ENTRY_TRAP       0xF

// IDT entry which maps a specific interrupt to an interrupt handler
//    base_lo   - Lower 16 bits giving location of handler
//    sel       - Offset of GDT entry of code segment handler will be ran in
//    flags     - See above
//    base_hi   - Upper 16 bits giing location of handler
struct idt_entry
{
    u16 base_lo;
    u16 sel;        /* Our kernel segment goes here! */
    u8 always0;     /* This will ALWAYS be set to 0! */
    u8 flags;       /* Set using the above table! */
    u16 base_hi;
} __attribute__((packed));

// A IDT pointer. This is the data structure that is used by the lidt and sidt
// instructions to register / return the location of the IDT in memory.
struct idt_ptr
{
    u16 limit;
    u32 base;
} __attribute__((packed));

// The main init function that inits the irq module
void irq_init();

// Register an interrupt handler???? Some thought needs to go here
void irq_register_PIC_handler(void (*handler)(void), u8 entry);

// Register a soft interrupt i.e. irq 48-63
void irq_register_soft(void (*handler)(void), u8 entry);
void irq_raise(u8  entry);

// public API funcs to turn on and off interrupts
void irq_on();
void irq_off();

#endif