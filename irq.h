#ifndef MEME_IRQ
#define MEME_IRQ

#include "types.h"

#define IRQ_EOI_MASTER_PORT  0x20
#define IRQ_EOI_SLAVE_PORT   0xA0
#define IRQ_EOI_MSG          0x20
#define IRQ_MASK_MASTER_PORT 0x21
#define IRQ_MASK_SLAVE_PORT  0x21

/* Defines an IDT entry */
struct idt_entry
{
    u16 base_lo;
    u16 sel;        /* Our kernel segment goes here! */
    u8 always0;     /* This will ALWAYS be set to 0! */
    u8 flags;       /* Set using the above table! */
    u16 base_hi;
} __attribute__((packed));

struct idt_ptr
{
    u16 limit;
    u32 base;
} __attribute__((packed));

void irq_get_curr_idt_ptr(struct idt_ptr *p);
void irq_init_idt();
void irq_raise33();

#endif