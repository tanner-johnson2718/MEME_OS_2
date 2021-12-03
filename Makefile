CC = gcc

LDFLAGS = -T link.ld -melf_i386

all: kernel.elf

kernel_entry: kernel_entry.c
	gcc -m32 -c kernel_entry.c -o kernel_entry.o

asm_entry: entry.S
	gcc -m32 -c entry.S -o entry.o

kernel.elf: asm_entry kernel_entry
	ld $(LDFLAGS) entry.o kernel_entry.o -o kernel.elf

clean:
	rm -f *.o *.elf
	rm -f kernel.elf