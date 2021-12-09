#ifndef MEME_IRQ
#define MEME_IRQ

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

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
struct idt_entry idt[256];
struct idt_ptr idtp;

/* This exists in 'start.asm', and is used to load our IDT */
void irq_init_idt();
void irq_reg_isr(u32 irq_num);
void irq_send_eoi(u32 irq_num);
void irq_toggle_mask(u32 irq_num);

#endif