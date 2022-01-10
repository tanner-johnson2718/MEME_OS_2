gnome-terminal -- gdb kernel.elf -x gdb_commands.txt

qemu-system-i386 -net nic,model=rtl8139 -kernel kernel.elf -s -S 