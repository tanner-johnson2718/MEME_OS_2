CC = gcc

LDFLAGS = -T link.ld -melf_i386
CFLAGS = -m32 -fno-pie -fno-stack-protector

all: kernel.elf

kernel_entry: kernel_entry.c
	gcc $(CFLAGS) -c kernel_entry.c -o kernel_entry.o

asm_entry: entry.S
	gcc $(CFLAGS)  -c entry.S -o entry.o

serial: serial.h serial.c
	gcc $(CFLAGS) -c serial.c -o serial.o

gdt: gdt.c gdt.h
	gcc $(CFLAGS) -c gdt.c -o gdt.o

irq: irq.c irq.h
	gcc $(CFLAGS) -c irq.c -o irq.o

ps2: ps2.c ps2.h
	gcc $(CFLAGS) -c ps2.c -o ps2.o

kernel.elf: asm_entry kernel_entry serial gdt irq ps2
	ld $(LDFLAGS) entry.o kernel_entry.o serial.o gdt.o irq.o ps2.o -o kernel.elf

clean:
	rm -f *.o *.elf