gnome-terminal -- gdb kernel.elf -x gdb_commands.txt

qemu-system-i386 -net nic,model=i82551 -kernel kernel.elf -s -S 