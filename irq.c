#include "irq.h"

void idt_load()
{
    asm volatile("lidt (%0)" :: "g" (&idtp));
}

void idt_set_gate(u8 num, u32 base, u8 sel, u8flags)
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

/* Installs the IDT */
void irq_init_idt()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    /* Clear out the entire IDT, initializing it to zeros */
    u32 i = 0;
    for(i = 0; i < sizeof(struct idt_entry) * 256; ++i)
    {
        u8* p = (char*) &idt;
        p[i] = 0;
    }

    /* Points the processor's internal register to the new IDT */
    idt_load();
}