#include "core/types.h"
#include "drivers/vga.h" 

u16* get_loc_text_buffer(u8 i, u8 j)
{
    if(i >= VGA_BUFFER_WIDTH || j >= VGA_BUFFER_HEIGHT)
    {
        return 0x0;
    }

    u16 index = (j * VGA_BUFFER_WIDTH) + i;
    return ((u16*)(VGA_TEXT_BUFFER)) + index;
}

u16 build_buffer_entry(u8 bg, u8 fg, u8 c)
{
    u16 ret = c;
    ret |= (fg << VGA_FG_SHIFT);
    ret |= (bg << VGA_BG_SHIFT);
    return ret;
}

void put_entry_text_buff(u8 bg, u8 fg, u8 c, u8 i, u8 j)
{
    u16 entry = build_buffer_entry(bg, fg, c);
    u16* loc_16 = get_loc_text_buffer(i,j);
    if(loc_16 > 0)
    {
        *loc_16 = entry;
    }
    
}

// Public

void vga_textmode_putc(u8 x, u8 y, u8 c, u8 fg, u8 bg)
{
    put_entry_text_buff(bg, fg, c, x, y);
}

void vga_init()
{
    // Maybe verify that the VGA driver is set to fucking text mode?
    // Maybe allow input to allow different color modes? actually set pixels
}