#include "irq.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

void irq_idt_load()
{
    asm volatile("lidt %0" :: "m" (idtp));
}

void irq_get_curr_idt_ptr(struct idt_ptr *p)
{

    asm volatile("sidt (%0)" : "=a"(p):);
}

void irq_raise49()
{
    asm volatile("int $49" );
}


void irq_idt_set_gate(u8 num, u32 base, u8 sel, u8 flags)
{
    /* The interrupt routine's base address */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void irq_isr_sink()
{
    while(1)
    {

    }
}

/* Installs the IDT */
void irq_init_idt()
{

    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = (u32) &idt;

    /* Clear out the entire IDT, initializing it to zeros */
    u32 i = 0;
    for(i = 0; i < 256; ++i)
    {
        idt[i].base_lo = 0;
        idt[i].sel = 0;
        idt[i].always0 = 0;
        idt[i].flags = 0;
        idt[i].base_hi = 0;
    }

    // set up the first 32 exception interrupts
    for(i = 0; i < 256; ++i)
    {
        irq_idt_set_gate(i, (u32) irq_isr_sink, 0x08, 0x8E);
    }

    irq_idt_load();
}