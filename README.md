# MEME OS 2 (Electric Bugalo)
First MEME OS repository will be used to examine the Linux kernel. This project
will be the actual creation of an OS from as scratch as possible. For now
target will be the x86-64 architecture and will target the qemu virtual env.

# Boot / Build Process (Pre-Kernel)
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
  Modules name i.e. serial.h
- All "public" functions used to interface the module will have that module
  name as a prefix i.e. gdt_init() where gdt is the module name.
- All pre-proc macros will again have a the module name as a prefix
- All "public" functions will be declared in the header.
- Functions private to a module will not be declared in the header.
- Curly braces on new lines
- Minimize magic numbers using pre-proc macros (as readability dictates)
- All basic types will be those defined in types.h i.e. u32 or s16.

# Design
- Kernel will be composed of modules i.e. interrupt handler, serial driver,
  etc.
- All interrupts are handled in kernel code
- This implies all drivers will be in kernel which also implies monolithic
- Core will not be interoperable and will target the i386 arch.
- Drivers should be interoperable and should be able to target any arch

## Driver Model
- ??

# Modules
- GDT
- IRQ
- SERIAL
- PS2