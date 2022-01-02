# MEME OS 2
First MEME OS repository will be used to examine the Linux kernel. This project
will be the actual creation of an OS from as scratch as possible. For now
target will be the x86 architecture and will target the qemu virtual env.
This is sort of a "make it up" as you go, depth first search of OS dev.

**Jan 1 2022** - Big refactor. Message passing scheduler made things way too
complicated. Removing schduler, adding a kernel logger, and really really
cleaning driver code, especially making sure that 1) error codes checked, 2) 
errors are logged, 3) all kernel public functions a driver exports is clean,
well documentated, and always returns error codes. Will move on from there.

# Design
- Kernel will be composed of modules i.e. interrupt handler, serial driver,
  etc. (Maybe )
- Kernel is monolithic and all drivers and IRQ handlers reside in kernel.
- Single core
- For now everything resides in kernel
- Functionality will be created by simply calling driver public API's.

# Interrupt Table

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

# Drivers
- GDT
- IRQ
- SERIAL
- PS2
- VGA
- Timer

# Coding Style
- ... Fill me in

# Deps
- gcc
- git
- qemu
- qemu-kvm
- ubuntu build / vm host