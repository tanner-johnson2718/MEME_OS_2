# MEME OS 2 (Electric Bugalo)
First MEME OS repository will be used to examine the Linux kernel. This project
will be the actual creation of an OS from as scratch as possible. For now
target will be the x86-64 architecture and will target the qemu virtual env.

# Boot Process (Pre-Kernel)
- Linker Map
- Build Process
- ELF file
- Multiboot spec + magic number header

# Boot Process (Kernel)
- Serial port Init
- GDT Init
- IRQ Init

# Coding Style
- Distinct Modules will have their own .c and .h file(s) whose name the 
  Modules name i.e. uart_8250.h
- All "public" functions used to interface the module will have that module
  name as a prefix i.e. gdt_init() where gdt is the module name
- All "public" functions will be declared in the header.
- Functions private to a module will not be declared in the header.
- Curly braces on new lines
- Minimize magic numbers using pre-proc macros (as readability dictates)
- All basic types will be those defined in types.h i.e. u32 or s16.

  # Design
- Kernel will be composed of modules i.e. interrupt handler, uart 8250 driver,
  etc.
- All modules will have a defined output stream for logging that can be
  redirected as needed.
- All interrupts are handled in kernel code