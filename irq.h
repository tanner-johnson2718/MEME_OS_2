#ifndef MEME_IRQ
#define MEME_IRQ

#define IRQ_EOI_MASTER_PORT  0x20
#define IRQ_EOI_SLAVE_PORT   0xA0
#define IRQ_EOI_MSG          0x20
#define IRQ_MASK_MASTER_PORT 0x21
#define IRQ_MASK_SLAVE_PORT  0x21

void irq_reg_isr(int irq_num);
void irq_send_eoi(int irq_num);
void irq_toggle_mask(int irq_num);

#endif