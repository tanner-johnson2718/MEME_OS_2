#include "types.h"
#include "vga.h"
#include "io_port.h"
#include "log.h"

// See vga.h for "high level" documentation

///////////////////////////////////////////////////////////////////////////////
// Private Register and Macros
///////////////////////////////////////////////////////////////////////////////
#define VGA_REG0         0x3C0
#define VGA_REG1         0x3C1
#define VGA_REG3         0x3DA

// MISC output register
#define VGA_MISC_OUT_WRITE 0x3C2
#define VGA_MISC_OUT_READ  0x3CC

// Most used index registers
#define VGA_INDEX_REG0 0x3C4
#define VGA_DATA_REG0  0x3C5
#define VGA_INDEX_REG1 0x3CE
#define VGA_DATA_REG1  0x3CF
#define VGA_INDEX_REG2 0x3D4
#define VGA_DATA_REG2  0x3D5

// DAC registers
#define VGA_DAC_MASK        0x3C6
#define VGA_DAC_READ_INDEX  0x3C7
#define VGA_DAC_WRITE_INDEX 0x3C8
#define VGA_DAC_DATA        0x3C9

// Text buffer macros
#define VGA_TEXT_BUFFER   0xB8000
#define VGA_BG_SHIFT      12
#define VGA_FG_SHIFT      8
#define VGA_CHAR_SHIFT    0

///////////////////////////////////////////////////////////////////////////////
// Private Functions
///////////////////////////////////////////////////////////////////////////////

u16* get_loc_text_buffer(u8 i, u8 j)
{
    // Priavte function assume indicies are good

    u16 index = (j * VGA_BUFFER_WIDTH) + i;
    return ((u16*)(VGA_TEXT_BUFFER)) + index;
}

u16 build_buffer_entry(u8 bg, u8 fg, u8 c)
{
    // Priavte function assume input values are good

    u16 ret = c;
    ret |= (fg << VGA_FG_SHIFT);
    ret |= (bg << VGA_BG_SHIFT);
    return ret;
}

void put_entry_text_buff(u8 bg, u8 fg, u8 c, u8 i, u8 j)
{
    // Priavte function assume input values are good

    u16 entry = build_buffer_entry(bg, fg, c);
    u16* loc_16 = get_loc_text_buffer(i,j);
    if(loc_16 > 0)
    {
        *loc_16 = entry;
    }
    
}

// 0x1 - Bad buffer 
u8 check_input(u8 x, u8 y, u8 fg, u8 bg)
{
    if(x >= VGA_BUFFER_WIDTH || y >= VGA_BUFFER_HEIGHT)
    {
        return VGA_OUT_BOUNDS_INDICES;
    }

    if(fg > VGA_WHITE | bg > VGA_WHITE)
    {
        return VGA_INVALID_COLOR;
    }

    return 0x0;
}

///////////////////////////////////////////////////////////////////////////////
// Public
///////////////////////////////////////////////////////////////////////////////

/******************************************************************************
NAME)     vga_init

INPUTS)   NONE

OUTPUTS)  NONE

RETURNS)  NONE

COMMENTS) NONE
******************************************************************************/
u8 vga_init()
{
    // Maybe verify that the VGA driver is set to text mode?
    // Maybe allow input to allow different color modes? actually set pixels

    log_msg(__FILE__,__LINE__,"VGA successfully init-ed");

    return 0;
}




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
u8 vga_textmode_putc(u8 x, u8 y, u8 c, u8 fg, u8 bg)
{
    u8 ret = check_input(x,y,bg,fg);
    if(ret)
    {
        log_msg(__FILE__,__LINE__,"VGA bad input");
        return ret;
    }

    put_entry_text_buff(bg, fg, c, x, y);
    return 0;
}



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
u8 vga_textmode_getc(u8 x, u8 y, u8* c)
{
    u8 ret = check_input(x,y,0,0);
    if(ret)
    {
        log_msg(__FILE__,__LINE__,"VGA bad input");
        return ret;
    }

    u16* entry = get_loc_text_buffer(x, y);
    *c = (u8) ((*entry) & 0xff);
    return 0;
}




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
u8 vga_textmode_get_fg(u8 x, u8 y, u8* fg)
{
    u8 ret = check_input(x,y,0,0); 
    if(ret)
    {
        log_msg(__FILE__,__LINE__,"VGA bad input");
        return ret;
    }

    u16* entry = get_loc_text_buffer(x, y);
    *fg = (u8) (((*entry) >> VGA_FG_SHIFT) & 0xf);
    return 0;
}




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
u8 vga_textmode_get_bg(u8 x, u8 y, u8* bg)
{
    u8 ret = check_input(x,y,0,0);
    if(ret)
    {
        log_msg(__FILE__,__LINE__,"VGA bad input");
        return ret;
    }

    u16* entry = get_loc_text_buffer(x, y);
    *bg = (u8) (((*entry) >> VGA_BG_SHIFT) & 0xf);
    return 0;
}



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
u8 vga_textmode_clear_screen(u8 fg, u8 bg)
{
    u8 ret = check_input(0,0,fg,bg);
    if(ret)
    {
        log_msg(__FILE__,__LINE__,"VGA bad input");
        return ret;
    }

    u8 i, j;
    for(i = 0; i < VGA_BUFFER_HEIGHT; ++i)
    {
        for(j = 0; j < VGA_BUFFER_WIDTH; ++j)
        {
            ret = vga_textmode_putc(j, i, ' ', fg, bg);
            if(ret)
            {
                log_msg(__FILE__,__LINE__,"VGA bad input");
                return ret;
            }
        }
    }

    return 0;
}



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
u8 vga_textmode_update_cursor(u8 x, u8 y)
{

    u8 ret = check_input(x,y,0,0);
    if(ret)
    {
        log_msg(__FILE__,__LINE__,"VGA bad input");
        return ret;
    }

	u16 pos = y * VGA_BUFFER_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (u8) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (u8) ((pos >> 8) & 0xFF));

    return 0;
}