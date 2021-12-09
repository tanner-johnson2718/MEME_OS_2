CC = gcc

LDFLAGS = -T link.ld -melf_i386

all: kernel.elf

kernel_entry: kernel_entry.c
	gcc -m32 -c kernel_entry.c -o kernel_entry.o

asm_entry: entry.S
	gcc -m32 -c entry.S -o entry.o

uart_8250: uart_8250.h uart_8250.c
	gcc -m32 -fno-stack-protector -c uart_8250.c -o uart_8250.o

gdt: gdt.c gdt.h
	gcc -m32 -c gdt.c -o gdt.o

kernel.elf: asm_entry kernel_entry uart_8250 gdt
	ld $(LDFLAGS) entry.o kernel_entry.o uart_8250.o gdt.o -o kernel.elf

clean:
	rm -f *.o *.elf