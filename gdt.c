// See gdt.h for documentation

#include "gdt.h"

// Globals
struct gdt_entry gdt[3];
struct gdt_ptr gp;

void gdt_flush()
{
    asm volatile("lgdt %0" :: "m" (gp));
}

void gdt_get_curr_gdt_ptr(struct gdt_ptr *p)
{

    asm volatile("sgdt (%0)" : "=a"(p):);
}

/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(u32 num, u32 base, u32 limit, u8 access, u8 gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

/* Should be called by main. This will setup the special GDT
*  pointer, set up the first 3 entries in our GDT, and then
*  finally call gdt_flush() in our assembler file in order
*  to tell the processor where the new GDT is and update the
*  new segment registers */
void gdt_install()
{
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (u32) &gdt;

    /* Our NULL descriptor */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* We are just gonna set the entire 4GiB in the same code and data segment.
       Sounds as if the GDT isn't really used anymore. */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Do the same for userpace code
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    /* Flush out the old GDT and install the new changes! */
    gdt_flush();
}