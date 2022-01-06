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
- Functionality will be created by simply calling a drivers public API.
- For now just a collection of drivers with a main that acts as a minimal
  console

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
- Drivers export public API's
- All public functions return an error code with these codes being defined the
  drivers header
- All private functions, macros, types, registers, etc. should be defined in
  the c file
- Maximize private and make only public what makes sense to use the driver
- Segment code using code segment below
- All public functions should have a comment block as shown below
- Curly braces on newline
- See vga.c and vga.h for an example of coding style
- Public comment block and code segment template:
~~~
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
~~~

# Deps
- gcc
- git
- qemu
- qemu-kvm
- ubuntu build and vm host