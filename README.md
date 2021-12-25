# MEME OS 2 (Electric Bugalo)
First MEME OS repository will be used to examine the Linux kernel. This project
will be the actual creation of an OS from as scratch as possible. For now
target will be the x86 architecture and will target the qemu virtual env.
This is sort of a "make it up" as you go, depth first search of OS dev.

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
  interrupted for scheduling purposes

## Driver / Scheduler Model (The Completely Event Driven Scheduler)
- Interrupts drive all comptutation and scheduling events
- The driver / scheduler follows a PUB / SUB model.
  - On input events, an interrupt triggers driver code that publishes data to 
    be consumed by a publisher i.e. an application.
  - On output events an application pulishes data and raises an interrupt which
    triggers driver code (subscriber) which outputs the data on the medium.
- Drivers register their device specific routines w/ the irq module
  - This includes both input and output interrupts.
- Drivers are responsible for publishing data w/ the scheduler module (on input events)
- Scheduler has public functions for pushing driver events onto scheduler
  maintained buffers (on input events)
- Likewise, Scheduler has public functions for pushing output data onto scheduler
  maintained buffers (on output events)
- Scheduler also as public functions for raising output ready interrupts

## Interrupt Table

| Interrupt #      | Description                    | Type        | Driver |
| -----------      | -----------                    | ----------- | ----------- |
| 0-31             | CPU Generated Exceptions       | Exception   | None  |
| 32               | Timer                          | PIC         | Timer |
| 33               | Keyboard input                 | PIC         | ps2   |
| 34               | Slave PIC                      | PIC         | None  |
| 35               | COM 1                          | PIC         | serial |
| 36               | COM 2                          | PIC         | serial |
| 37 - 47          | Unimplemented                  | PIC         | None   |
| 48-63            | Unimplemented                  | Soft        | None   |

## Data IN Model

## Data OUT Model

# Modules
- GDT
- IRQ
- SERIAL
- PS2
- VGA
- Timer
- Console
- Scheduler
