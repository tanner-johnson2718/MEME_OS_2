# MEME OS 2
First MEME OS repository will be used to examine the Linux kernel. This project
will be the actual creation of an OS from as scratch as possible. For now
target will be the x86 architecture and will target the qemu virtual env.
This is sort of a "make it up" as you go, depth first search of OS dev.

**End State:** To give this project a definable end state, this project will be
consiered complete when the following are realized. This project will be 
confined to targeting the x86 (i386) qemu virtual machine. Next MEME OS's in
the series will make the jump to actual hardware.
* [X] Basic Keyboard driver.
* [X] Interrrupt handler
* [X] System timer and system clock. For simplcity just have a single counter
      at the ms granularity. No need to get fancy with this.
* [X] Basic textmode VGA driver.
* [X] Serial driver and logging functions.
* [X] PCI driver for discovering, enumerating, and initializing PCI devices and
      providing an interface for other pci drivers to interface with their PCI
      devices.
* [ ] Minimal, but powerful, console that can read memory, registers, modify mem
  and registers, and call executable code. Console will give minimal tools for
  examining the system. Almost a built-in debugger.
* [ ] PCI network driver working
* [ ] Network stack implemented from scratch up to sockets. Can talk to a linux 
  server TCP socket.
* [ ] USB Driver. Figure out how this intergaces with 
* [ ] MMIO / MMU memory manager. For this mostly just interface with any hardware
  to accelerate translation. No need to create full dynamic memory manager,
  just git warm fuzy for hardware / architecture on system todo memory stuff.
* [ ] **BONUS** full VGA PCI driver (full 16bit and 256bit color mode).
* [ ] **BONUS** locking and other sync primitives.

# Design
- Kernel will be composed of modules i.e. interrupt handler, serial driver,
  etc. (Maybe revisit this once the debug console is complete. Would be nice
  to be able load and unload drivers. VGA is a good example of this, unload
  the text mode driver and be able to load a full PCI version).
- Kernel is monolithic and all drivers and IRQ handlers reside in kernel.
- Single core
- For now everything resides in kernel
- Functionality will be created by simply calling a drivers public API.
- For now just a collection of drivers with a main that acts as a minimal
  console
- Built in debug console for examaining memory, call driver API funcs, etc.

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
- IRQ (8259 PIC)
- SERIAL (8250 UART)
- PS2
- VGA textmode
- Timer (8253/8254)
- PCI
- RTL8139 Ethernet Driver

# Coding Style
- All modules implementation are contained in a .c file with the module name
  as the .c file name i.e. vga.c. C files are contained in kernel/
- The public attributes (types, defines, API functions) should be containded
  in a header with the modules name as the header name i.e. vga.h and placed
  in include/kernel/.
- Drivers / Modules export public API's
- All public functions return an error code with these codes being defined in
  the drivers header
- All private functions, macros, types, registers, etc. should be defined in
  the c file
- Maximize private and make only public what makes sense to use the driver
- Segment code using code segment below
- All public functions should have a comment block as shown below
- Curly braces on newline
- Log any caught errors
- See vga.c and vga.h for an example of coding style
~~~C
///////////////////////////////////////////////////////////////////////////////
// Public Functions
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)    vga_textmode_getc

INPUTS)  
        0) u8 x  - X position in character matrix to put char. Must be between 
                   0 and VGA_BUFFER_WIDTH
        1) u8 y  - Y position in character matrix to put char. Must be between 
                   0 and VGA_BUFFER_HEIGHT
        
OUTPUTS)
         2) u8* c  - Pointer to a 1 byte buffer to store char at spec. loc.

RETURNS) 0 on success or VGA error code on error.

COMMENTS) NONE
******************************************************************************/
u8 vga_textmode_getc(u8 x, u8 y, u8* c);
~~~

# Deps
- gcc
- git
- qemu
- qemu-kvm
- ubuntu build and vm host