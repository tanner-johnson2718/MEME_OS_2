CC = gcc

LDFLAGS = -T link.ld -melf_i386
CFLAGS = -m32 -fno-pie -fno-stack-protector -I./

all: kernel.elf

kernel_entry: kernel_entry.c
	gcc $(CFLAGS) -c kernel_entry.c -o kernel_entry.o

asm_entry: entry.S
	gcc $(CFLAGS)  -c entry.S -o entry.o

serial: drivers/serial.h drivers/serial.c
	gcc $(CFLAGS) -c drivers/serial.c -o serial.o

gdt: core/gdt.c core/gdt.h
	gcc $(CFLAGS) -c core/gdt.c -o gdt.o

irq: core/irq.c core/irq.h
	gcc $(CFLAGS) -c core/irq.c -o irq.o

ps2: drivers/ps2.c drivers/ps2.h
	gcc $(CFLAGS) -c drivers/ps2.c -o ps2.o

kernel.elf: asm_entry kernel_entry serial gdt irq ps2
	ld $(LDFLAGS) entry.o kernel_entry.o serial.o gdt.o irq.o ps2.o -o kernel.elf

clean:
	rm -f *.o *.elf