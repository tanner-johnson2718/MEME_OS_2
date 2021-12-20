# MEME OS 2 (Electric Bugalo)
First MEME OS repository will be used to examine the Linux kernel. This project
will be the actual creation of an OS from as scratch as possible. For now
target will be the x86 architecture and will target the qemu virtual env.

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
- (TODO) Unified error handling i.e. all public functions return an
  error code and all outputs are passed as pointers.

# Design
- Kernel will be composed of modules i.e. interrupt handler, serial driver,
  etc.
- All interrupts are handled in kernel code
- This implies all drivers will be in kernel which also implies monolithic
- Architecture specific code will be put in "core". Although not all core code
  is necessarily architecture specific.
- Drivers should be interoperable and should be able to target any arch
- For know will be sinlge core (TODO add SMP support)
- Non-Pre-emptive : applications are expected to yield and will not be 
  interrupted

## Driver / Scheduler Model (The Completely Event Driven Scheduler)
- The main CPU thread sits idle
- Interrupts drive all comptutation and scheduling events
- The driver / scheuler model follows a PUB / SUB modole where applications 
  (subs) suncribe to data streams from drivers (publishers).
- Interrupts 0-31 are CPU generated excpetions and do not follow this model
- Interrupts 32-47 are PIC generated interrupts for things such as keyboard, 
  serial port, etc.
- Interrupts 48 - 256 will be reserved for software generated interrupts
- For interrupts 32+, the irq base handler is called
- Drivers register their device specific routines using irq public functions
  which are subsequently called by the base handler. 
- Drivers are responsible for publishing data
- Scheduler has public functions for pushing driver events onto scheduler
  maintained buffers
- Driver returns to irq base handler, which subsequently returns
- Main CPU thread resumes execution and is kicked out of its idle
- Main CPU thread invokes scheduler to check for any new publications
- Scheduler invokes any applications that are subscribed to the new pubs
- When all applications yield the scheduler will check if any more data pubs
  came in and if not will idle.
- ?????? the irq base handler and driver handler cannot be interrupted, but the
  scheduler and applications can make sure this is handled.

# Modules
- GDT
- IRQ
- SERIAL
- PS2
- VGA
- Console