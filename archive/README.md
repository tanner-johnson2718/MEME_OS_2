# MEME OS 2 (Electric Bugalo)
First MEME OS repository will be used to examine the Linux kernel. This project
will be the actual creation of an OS from as scratch as possible. For now
target will be the x86 architecture and will target the qemu virtual env.
This is sort of a "make it up" as you go, depth first search of OS dev.

# Design
- Kernel will be composed of modules i.e. interrupt handler, serial driver,
  etc.
    - eh, is this really even enforced, what about sched?
- Kernel is monolithic and all drivers and intial IRQ handlers reside in kernel.
- For know will be sinlge core (TODO add SMP support)
- Non-Pre-emptive : applications are expected to yield and will not be 
  interrupted for scheduling purposes
- Interrupts drive all comptutation and scheduling events and IO events form the basis of the OS
- Kernel space has a public API which includes drivers
- User space only communciates with the kernel via message passing through the
  scheduler i.e. the public API the scheduler exports to user space.
  - The scheduler and eventually the mmu may be bundled into a "middle-ware" layer

## Scheduler Model (The Completely IO Driven Scheduler)
The work flow for an IO event, and subsequently a scheduler invocation are)
- Hardware (TODO software later) generates an interrupt
- IRQ module catches it and calls driver code registered to handle that interrupt
- Driver code is responible for publishing input data w/ scheduler
  - This puts an event on scheduler maintained buffers
- Driver code returns, IRQ code cleans up (sends EOI to PIC, etc)
  - Now entering interruptable territory 
- Main CPU thread is awoken and the scheduler thread is invoked
- Scheduler calls all registered application call backs waiting on data
- Applications execute and publish output data, targeting a driver
- All Applications yield (of their own accord)
- Scheduler invokes driver callbacks to output application published data
- Main CPU thread invoked and returns to idle

## Data / IO Model

## Kernel Block Diagram

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


# Modules??
- GDT
- IRQ
- SERIAL
- PS2
- VGA
- Timer
- Console
- Scheduler

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

# Deps
- gcc
- git
- qemu
- qemu-kvm
- ubuntu build / vm host
