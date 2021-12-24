#include "core/irq.h"
#include "core/gdt.h"
#include "core/io_port.h"

struct idt_entry idt[256];
struct idt_ptr idtp;
void* irq_pic_handlers[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void* irq_soft_handlers[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
void* irq_soft_raise[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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

// First 32 interrupts are CPU driven i.e. exceptions
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

// IRQs driven by pic
void irq_common_pic_handler(u8 irq_num)
{
    if(irq_pic_handlers[irq_num])
    {
        void (*handler)() = irq_pic_handlers[irq_num];
        handler();
    }

    if(irq_num >= 8)
    {
        outb(IRQ_PIC_SLAVE_CMD_PORT, IRQ_PIC_EOI);
    }

    outb(IRQ_PIC_MASTER_CMD_PORT, IRQ_PIC_EOI);
}

void irq_isr_sink32()
{
    irq_common_pic_handler(0);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink33()
{
    irq_common_pic_handler(1);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink34()
{
    irq_common_pic_handler(2);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink35()
{
    irq_common_pic_handler(3);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink36()
{
    irq_common_pic_handler(4);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink37()
{
    irq_common_pic_handler(5);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink38()
{
    irq_common_pic_handler(6);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink39()
{
    irq_common_pic_handler(7);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink40()
{
    irq_common_pic_handler(8);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink41()
{
    irq_common_pic_handler(9);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink42()
{
    irq_common_pic_handler(10);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink43()
{
    irq_common_pic_handler(11);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink44()
{
    irq_common_pic_handler(12);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink45()
{
    irq_common_pic_handler(13);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink46()
{
    irq_common_pic_handler(14);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink47()
{
    irq_common_pic_handler(15);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_common_soft_handler(u8 irq_num)
{
    if(irq_soft_handlers[irq_num])
    {
        void (*handler)() = irq_soft_handlers[irq_num];
        handler();
    }
}

void irq_isr_sink48()
{
    irq_common_soft_handler(0);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink49()
{
    irq_common_soft_handler(1);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink50()
{
    irq_common_soft_handler(2);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink51()
{
    irq_common_soft_handler(3);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink52()
{
    irq_common_soft_handler(4);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink53()
{
    irq_common_soft_handler(5);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink54()
{
    irq_common_soft_handler(6);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink55()
{
    irq_common_soft_handler(7);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink56()
{
    irq_common_soft_handler(8);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink57()
{
    irq_common_soft_handler(9);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink58()
{
    irq_common_soft_handler(10);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink59()
{
    irq_common_soft_handler(11);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink60()
{
    irq_common_soft_handler(12);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink61()
{
    irq_common_soft_handler(13);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink62()
{
    irq_common_soft_handler(14);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_isr_sink63()
{
    irq_common_soft_handler(15);
    asm volatile("leave");
    asm volatile("iret");
}

void irq_raise48()
{
    asm volatile("int $48");
}

void irq_raise49()
{
    asm volatile("int $49");
}

void irq_raise50()
{
    asm volatile("int $50");
}

void irq_raise51()
{
    asm volatile("int $51");
}

void irq_raise52()
{
    asm volatile("int $52");
}

void irq_raise53()
{
    asm volatile("int $53");
}

void irq_raise54()
{
    asm volatile("int $54");
}

void irq_raise55()
{
    asm volatile("int $55");
}

void irq_raise56()
{
    asm volatile("int $56");
}

void irq_raise57()
{
    asm volatile("int $57");
}

void irq_raise58()
{
    asm volatile("int $58");
}

void irq_raise59()
{
    asm volatile("int $59");
}

void irq_raise60()
{
    asm volatile("int $60");
}

void irq_raise61()
{
    asm volatile("int $61");
}

void irq_raise62()
{
    asm volatile("int $62");
}

void irq_raise63()
{
    asm volatile("int $63");
}

void irq_raise(u8  entry)
{
    if(entry >= 0x30 && entry < 0x40)
    {
        if(irq_soft_raise[entry - 0x30])
        {
            void (*raise)() = irq_soft_raise[entry - 0x30];
            raise();
        }
    }
}

void irq_register_handler(void (*handler)(void), u8 entry)
{
    irq_idt_set_gate(entry, (u32) handler, GDT_KERNEL_CODE_SECTOR, 0x8E);
}

void irq_register_PIC_handler(void (*handler)(void), u8 entry)
{
    if(entry >= 0x20 && entry <= 0x2F)
    {
        // registering a PIC handler
        irq_pic_handlers[entry - 0x20] = handler;
    }
}

void irq_register_soft(void (*handler)(void), u8 entry)
{
    if(entry < 0x30 && entry >= 0x40)
    {
        return;   // error
    }

    irq_soft_handlers[entry - 0x30] = handler;
}

// The first 32 exceptions are reserved but the 8259 PIC wants to generate
// interrupts 0-15. Need to remap these to 32-47.
void irq_remap(void)
{
    // starts the initialization sequence (in cascade mode)
	outb(IRQ_PIC_MASTER_CMD_PORT, IRQ_PIC_ICW1_INIT | IRQ_PIC_ICW1_ICW4);  
	outb(IRQ_PIC_SLAVE_CMD_PORT, IRQ_PIC_ICW1_INIT | IRQ_PIC_ICW1_ICW4);

    // Map master irqs starting at 32
	outb(IRQ_PIC_MASTER_DATA_PORT, IRQ_PIC_MASTER_BASE);                 
	
    // Map slave irqs starting at 40
	outb(IRQ_PIC_SLAVE_DATA_PORT, IRQ_PIC_SLAVE_BASE);                 
	
    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(IRQ_PIC_MASTER_DATA_PORT, 4);                       
	
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
	outb(IRQ_PIC_SLAVE_DATA_PORT, 2);                       
 
	outb(IRQ_PIC_MASTER_DATA_PORT, IRQ_PIC_ICW4_8086);
	outb(IRQ_PIC_SLAVE_DATA_PORT, IRQ_PIC_ICW4_8086);
 
    // clear saved masks.
	outb(IRQ_PIC_MASTER_DATA_PORT, 1);   
	outb(IRQ_PIC_SLAVE_DATA_PORT, 1);
}

void irq_init()
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

    // PIC interrupts
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

    // soft interrupts
    irq_register_handler(irq_isr_sink48, 48);
    irq_register_handler(irq_isr_sink49, 49);
    irq_register_handler(irq_isr_sink50, 50);
    irq_register_handler(irq_isr_sink51, 51);
    irq_register_handler(irq_isr_sink52, 52);
    irq_register_handler(irq_isr_sink53, 53);
    irq_register_handler(irq_isr_sink54, 54);
    irq_register_handler(irq_isr_sink55, 55);
    irq_register_handler(irq_isr_sink56, 56);
    irq_register_handler(irq_isr_sink57, 57);
    irq_register_handler(irq_isr_sink58, 58);
    irq_register_handler(irq_isr_sink59, 59);
    irq_register_handler(irq_isr_sink60, 60);
    irq_register_handler(irq_isr_sink61, 61);
    irq_register_handler(irq_isr_sink62, 62);
    irq_register_handler(irq_isr_sink63, 63);

    // set up soft raise code
    irq_soft_raise[0] = irq_raise48;
    irq_soft_raise[1] = irq_raise49;
    irq_soft_raise[2] = irq_raise50;
    irq_soft_raise[3] = irq_raise51;
    irq_soft_raise[4] = irq_raise52;
    irq_soft_raise[5] = irq_raise53;
    irq_soft_raise[6] = irq_raise54;
    irq_soft_raise[7] = irq_raise55;
    irq_soft_raise[8] = irq_raise56;
    irq_soft_raise[9] = irq_raise57;
    irq_soft_raise[10] = irq_raise58;
    irq_soft_raise[11] = irq_raise59;
    irq_soft_raise[12] = irq_raise60;
    irq_soft_raise[13] = irq_raise61;
    irq_soft_raise[14] = irq_raise62;
    irq_soft_raise[15] = irq_raise63;

    // point the cpu to our IDT
    irq_idt_load();

    // Remap the PIC 8259 interrupts to 32-47
    irq_remap();
    
}

void irq_on()
{
    asm volatile("sti");
}

void irq_off()
{
    asm volatile("cli");
}