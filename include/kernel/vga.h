#ifndef MEME_VGA
#define MEME_VGA

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Documentation
///////////////////////////////////////////////////////////////////////////////

/* Text mode vga driver - The following driver uses the VGA in the 25x80 char.
   mode. It assume the bootloader set the VGA controller to operate in this way.
   There is a char. matrix stored at 0xB8000 storing 16 bit values in a 25 by 
   80 matrix array. The upper 4 bits specify the background, the next 4 the
   foreground and the lower 8 bits the ascii character. The driver simply 
   exposes kernel public API functions for setting entries in this matrix.

   See docs/BKD_VGA.htm and "docs/VGA Hardware - OSDev Wiki.html" for the
   relevant documentation.
*/

///////////////////////////////////////////////////////////////////////////////
// Kernel Public VGA defines
///////////////////////////////////////////////////////////////////////////////

// VGA ERROR Codes
#define VGA_OUT_BOUNDS_INDICES 0x1
#define VGA_INVALID_COLOR      0x2

// 16 Bit color values
#define VGA_BLACK    0x0
#define VGA_BLUE     0x1
#define VGA_GREEN    0x2
#define VGA_CYAN     0x3
#define VGA_RED      0x4
#define VGA_MAGENTA  0x5
#define VGA_BROWN    0x6
#define VGA_LGRAY    0x7
#define VGA_DGRAY    0x8
#define VGA_LBLUE    0x9
#define VGA_LGREEN   0xA
#define VGA_LCYAN    0xB
#define VGA_LRED     0xC
#define VGA_LMAGENTA 0xD
#define VGA_LBROWN   0xE
#define VGA_WHITE    0xF

// How many ascii chars I can fit
#define VGA_BUFFER_WIDTH  80
#define VGA_BUFFER_HEIGHT 25


///////////////////////////////////////////////////////////////////////////////
// Kernel VGA API
///////////////////////////////////////////////////////////////////////////////


/******************************************************************************
NAME)     vga_init

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  0 always succeeds

COMMENTS) NONE
******************************************************************************/
u8 vga_init();




/******************************************************************************
NAME)    vga_textmode_putc

INPUTS)  
        0) u8 x  - X position in character matrix to put char. Must be between 
                   0 and VGA_BUFFER_WIDTH
        1) u8 y  - Y position in character matrix to put char. Must be between 
                   0 and VGA_BUFFER_HEIGHT
        2) u8 c  - The character to place on the character matrix
        3) u8 fg - Foreground color. Must be between min and max color code.
                   see vga_msg.h.
        4) u8 bg - Background color. Must be between min and max color code.
                   see vga_msg.h.

OUTPUTS) NONE

RETURNS) 0 on success or VGA error code on error.

COMMENTS) NONE
******************************************************************************/
u8 vga_textmode_putc(u8 x, u8 y, u8 c, u8 fg, u8 bg);



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



/******************************************************************************
NAME)    vga_textmode_get_fg

INPUTS)  
        0) u8 x  - X position in character matrix to put char. Must be between 
                   0 and VGA_BUFFER_WIDTH
        1) u8 y  - Y position in character matrix to put char. Must be between 
                   0 and VGA_BUFFER_HEIGHT
        
OUTPUTS)
         2) u8* fg  - Pointer to a 1 byte buffer to store fg at spec. loc.

RETURNS) 0 on success or VGA error code on error.

COMMENTS) NONE
******************************************************************************/
u8 vga_textmode_get_fg(u8 x, u8 y, u8* fg);




/******************************************************************************
NAME)    vga_textmode_get_bg

INPUTS)  
        0) u8 x  - X position in character matrix to put char. Must be between 
                   0 and VGA_BUFFER_WIDTH
        1) u8 y  - Y position in character matrix to put char. Must be between 
                   0 and VGA_BUFFER_HEIGHT
        
OUTPUTS)
         2) u8* bg  - Pointer to a 1 byte buffer to store fg at spec. loc.

RETURNS) 0 on success or VGA error code on error.

COMMENTS) NONE
******************************************************************************/
u8 vga_textmode_get_bg(u8 x, u8 y, u8* bg);




/******************************************************************************
NAME)    vga_textmode_clear_screen

INPUTS)  
        0) u8 fg - Foreground color. Must be between min and max color code.
                   see vga_msg.h.
        1) u8 bg - Background color. Must be between min and max color code.
                   see vga_msg.h.
        
OUTPUTS) NONE

RETURNS) 0 on success or VGA error code on error.

COMMENTS) Just puts a space on every location of char matrix w/ spec. bg and fg
******************************************************************************/
u8 vga_textmode_clear_screen(u8 fg, u8 bg);



/******************************************************************************
NAME)    vga_textmode_update_cursor


INPUTS)  
        0) u8 x  - X position in character matrix to put char. Must be between 
                   0 and VGA_BUFFER_WIDTH
        1) u8 y  - Y position in character matrix to put char. Must be between 
                   0 and VGA_BUFFER_HEIGHT
        
OUTPUTS)
         2) u8* bg  - Pointer to a 1 byte buffer to store fg at spec. loc.

RETURNS) 0 on success or VGA error code on error.

COMMENTS) NONE
******************************************************************************/
u8 vga_textmode_update_cursor(u8 x, u8 y);

#endif