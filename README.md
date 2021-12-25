# MEME OS 2 (Electric Bugalo)
First MEME OS repository will be used to examine the Linux kernel. This project
will be the actual creation of an OS from as scratch as possible. For now
target will be the x86 architecture and will target the qemu virtual env.

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
- The main CPU thread sits idle
- Interrupts drive all comptutation and scheduling events
- The driver / scheduler follows a PUB / SUB model where applications 
  (subs) subcribe to data streams from drivers (publishers).
- Interrupts 0-31 are CPU generated excpetions and do not follow this model
- Interrupts 32-47 are PIC generated interrupts for things such as keyboard, 
  serial port, etc.
- Interrupts 48-63 are reserved for software interrupts.
- Drivers register their device specific routines w/ the irq module
- Drivers are responsible for publishing data w/ the scheduler module
- Scheduler has public functions for pushing driver events onto scheduler
  maintained buffers

## Interrupt Table

| Interrupt #      | Description                    | Driver |
| -----------      | -----------                    | ----------- |
| 0-31             | CPU Generated Exceptions       | None  |
| 32               | Timer                          | Timer |
| 33               | Keyboard input                 | ps2   |
| 34               | Slave PIC                      | None  |
| 35               | COM 1                          | serial |
| 36               | COM 2                          | serial |
| 37 - 63          | Unimplemented                  | None   |

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
