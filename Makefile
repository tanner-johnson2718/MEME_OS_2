LDFLAGS = -T kernel/link.ld -melf_i386

all : kernel.elf

FORCE:

kernel: FORCE
	make -C kernel/

apps: FORCE
	make -C apps/

kernel.elf: kernel apps
	ld $(LDFLAGS) kernel/*.o apps/*.o -o kernel.elf

clean:
	rm -f *.o *.elf
	make -C kernel clean
	make -C apps clean