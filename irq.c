#include "irq.h"
#include "gdt.h"
#include "io_port.h"

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

void irq_isr_sink0(){}
void irq_isr_sink1(){}
void irq_isr_sink2(){}
void irq_isr_sink3(){}
void irq_isr_sink4(){}
void irq_isr_sink5(){}
void irq_isr_sink6(){}
void irq_isr_sink7(){}
void irq_isr_sink8(){}
void irq_isr_sink9(){}
void irq_isr_sink10(){}
void irq_isr_sink11(){}
void irq_isr_sink12(){}
void irq_isr_sink13(){}
void irq_isr_sink14(){}
void irq_isr_sink15(){}
void irq_isr_sink16(){}
void irq_isr_sink17(){}
void irq_isr_sink18(){}
void irq_isr_sink19(){}
void irq_isr_sink20(){}
void irq_isr_sink21(){}
void irq_isr_sink22(){}
void irq_isr_sink23(){}
void irq_isr_sink24(){}
void irq_isr_sink25(){}
void irq_isr_sink26(){}
void irq_isr_sink27(){}
void irq_isr_sink28(){}
void irq_isr_sink29(){}
void irq_isr_sink30(){}
void irq_isr_sink31(){}
void irq_isr_sink32(){}
void irq_isr_sink33(){}
void irq_isr_sink34(){}
void irq_isr_sink35(){}
void irq_isr_sink36(){}
void irq_isr_sink37(){}
void irq_isr_sink38(){}
void irq_isr_sink39(){}
void irq_isr_sink40(){}
void irq_isr_sink41(){}
void irq_isr_sink42(){}
void irq_isr_sink43(){}
void irq_isr_sink44(){}
void irq_isr_sink45(){}
void irq_isr_sink46(){}
void irq_isr_sink47(){}

void irq_register_handler(void (*handler)(void), u8 entry)
{
    irq_idt_set_gate(entry, (u32) handler, KERNEL_CODE_SECTOR, 0x8E);
}

// The first 32 exceptions are reserved but the 8259 PIC wants to generate
// interrupts 0-15. Need to remap these to 32-47.
void irq_remap(void)
{
    // starts the initialization sequence (in cascade mode)
	outb(PIC_MASTER_CMD_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);  
	outb(PIC_SLAVE_CMD_PORT, PIC_ICW1_INIT | PIC_ICW1_ICW4);

    // Map master irqs starting at 32
	outb(PIC_MASTER_DATA_PORT, PIC_MASTER_BASE);                 
	
    // Map slave irqs starting at 40
	outb(PIC_SLAVE_DATA_PORT, PIC_SLAVE_BASE);                 
	
    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(PIC_MASTER_DATA_PORT, 4);                       
	
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
	outb(PIC_SLAVE_DATA_PORT, 2);                       
 
	outb(PIC_MASTER_DATA_PORT, PIC_ICW4_8086);
	outb(PIC_SLAVE_DATA_PORT, PIC_ICW4_8086);
 
    // clear saved masks.
	outb(PIC_MASTER_DATA_PORT, 1);   
	outb(PIC_SLAVE_DATA_PORT, 1);
}

void irq_init_idt()
{

    // Allow interrupts
    asm volatile("sti");

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

    // set up the first 48 exception interrupts
    irq_register_handler(irq_isr_sink0, 0);
    irq_register_handler(irq_isr_sink1, 1);
    irq_register_handler(irq_isr_sink2, 2);
    irq_register_handler(irq_isr_sink3, 3);
    irq_register_handler(irq_isr_sink4, 4);
    irq_register_handler(irq_isr_sink5, 5);
    irq_register_handler(irq_isr_sink6, 6);
    irq_register_handler(irq_isr_sink7, 7);
    irq_register_handler(irq_isr_sink8, 8);
    irq_register_handler(irq_isr_sink9, 9);
    irq_register_handler(irq_isr_sink10, 10);
    irq_register_handler(irq_isr_sink11, 11);
    irq_register_handler(irq_isr_sink12, 12);
    irq_register_handler(irq_isr_sink13, 13);
    irq_register_handler(irq_isr_sink14, 14);
    irq_register_handler(irq_isr_sink15, 15);
    irq_register_handler(irq_isr_sink16, 16);
    irq_register_handler(irq_isr_sink17, 17);
    irq_register_handler(irq_isr_sink18, 18);
    irq_register_handler(irq_isr_sink19, 19);
    irq_register_handler(irq_isr_sink20, 20);
    irq_register_handler(irq_isr_sink21, 21);
    irq_register_handler(irq_isr_sink22, 22);
    irq_register_handler(irq_isr_sink23, 23);
    irq_register_handler(irq_isr_sink24, 24);
    irq_register_handler(irq_isr_sink25, 25);
    irq_register_handler(irq_isr_sink26, 26);
    irq_register_handler(irq_isr_sink27, 27);
    irq_register_handler(irq_isr_sink28, 28);
    irq_register_handler(irq_isr_sink29, 29);
    irq_register_handler(irq_isr_sink30, 30);
    irq_register_handler(irq_isr_sink31, 31);
    irq_register_handler(irq_isr_sink32, 32);
    irq_register_handler(irq_isr_sink33, 33);
    irq_register_handler(irq_isr_sink34, 34);
    irq_register_handler(irq_isr_sink35, 35);
    irq_register_handler(irq_isr_sink36, 36);
    irq_register_handler(irq_isr_sink37, 37);
    irq_register_handler(irq_isr_sink38, 38);
    irq_register_handler(irq_isr_sink39, 39);
    irq_register_handler(irq_isr_sink40, 40);
    irq_register_handler(irq_isr_sink41, 41);
    irq_register_handler(irq_isr_sink42, 42);
    irq_register_handler(irq_isr_sink43, 43);
    irq_register_handler(irq_isr_sink44, 44);
    irq_register_handler(irq_isr_sink45, 45);
    irq_register_handler(irq_isr_sink46, 46);
    irq_register_handler(irq_isr_sink47, 47);

    // point the cpu to our IDT
    irq_idt_load();

    // Remap the PIC 8259 interrupts to 32-47
    irq_remap();

    // turn on interrupts
    asm volatile("sti");
}