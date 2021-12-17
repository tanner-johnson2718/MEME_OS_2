#ifndef MEME_IRQ
#define MEME_IRQ

#include "types.h"

// The IRQ module is the MEME OS 2 interrupt handler. This module first
// handles allocating and gives public functions for manipulating the IDT. The
// IDT associates an interrupt with a handler. All interrupts will be handled
// at least initially in kernel space i.e the IDT entry will execute in the 
// kernel code sector. All interrupts will be treated as interrupt as opposed
// to a trap.

// The second main function is to act as a PIC 8259 driver. Module will init
// this device, remap its interrupts to 32-47, and will initially mask the 
// timer interrupt until the timer moduler is inited and can handle it. 

// PIC 8259 Ports
#define PIC_MASTER_CMD_PORT  0x20
#define PIC_MASTER_DATA_PORT 0x21
#define PIC_SLAVE_CMD_PORT   0xA0
#define PIC_SLAVE_DATA_PORT  0xA1

// PIC 8259 Commands
#define PIC_ICW1_INIT 0x10
#define PIC_ICW1_ICW4 0x01   // dont need ICW4?
#define PIC_ICW4_8086 0x01   // 8086 mode?
#define PIC_EOI       0x20

// PIC Interupts
#define PIC_MASTER_BASE 0x20
#define PIC_SLAVE_BASE  0x28

#define PIC_TIMER       PIC_MASTER_BASE + 0x0
#define PIC_KEY         PIC_MASTER_BASE + 0x1
#define PIC_SLAVE       PIC_MASTER_BASE + 0x2
#define PIC_COM1        PIC_MASTER_BASE + 0x3
#define PIC_COM2        PIC_MASTER_BASE + 0x4
#define PIC_SOUND       PIC_MASTER_BASE + 0x5
#define PIC_FLOPPY      PIC_MASTER_BASE + 0x6
#define PIC_LPT1        PIC_MASTER_BASE + 0x7

#define PIC_RT_CLOCK    PIC_SLAVE_BASE + 0x0
#define PIC_IRQ2        PIC_SLAVE_BASE + 0x1
#define PIC_PCI1        PIC_SLAVE_BASE + 0x2
#define PIC_PCI2        PIC_SLAVE_BASE + 0x3
#define PIC_MOUSE       PIC_SLAVE_BASE + 0x4
#define PIC_MATH        PIC_SLAVE_BASE + 0x5
#define PIC_HD          PIC_SLAVE_BASE + 0x6
#define PIC_RESV        PIC_SLAVE_BASE + 0x7


///////////////////////////////////////////////////////////
// IDT entry flag  bit fields
///////////////////////////////////////////////////////////

// flags byte [7] - Present bit. 
#define IDT_ENTRY_PRESENT_SHIFT 0x7
#define IDT_ENTRY_PRESENT       0x1
#define IDT_ENTRY_NOT_PRESENT   0x0

// flags byte [5-6] - Privelege 
#define IDT_ENTRY_PRIVILEGE            0x5
#define IDT_ENTRY_KERNEL_PRIVLEGE      0x0
#define IDT_ENTRY_USER_PRIVLEGE        0x3

// flags byte [4] - Always 0

// flags byte [0-3] - Gate Type
#define IDT_ENTRY_TYPE_SHIFT 0x0
#define IDT_ENTRY_TASK       0x5
#define IDT_ENTRY_INTERRUPT  0xE
#define IDT_ENTRY_TRAP       0xF

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

// Get the location of the IDT that is registered with the CPU
void irq_get_curr_idt_ptr(struct idt_ptr *p);

// The main init function that inits the irq module
void irq_init();

// Register an interrupt handler???? Some thought needs to go here
void irq_register_handler(void (*handler)(void), u8 entry);
void irq_register_PIC_handler(void (*handler)(void), u8 entry);
// TODO masking functions

#endif