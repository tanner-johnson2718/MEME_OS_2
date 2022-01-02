LDFLAGS = -T kernel/link.ld -melf_i386

all : kernel.elf

FORCE:

kernel: FORCE
	make -C kernel/

kernel.elf: kernel
	ld $(LDFLAGS) kernel/*.o -o kernel.elf

clean:
	rm -f *.o *.elf
	make -C kernel clean
