#include "core/types.h"
#include "drivers/vga.h"

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

// Public

void vga_textmode_putc(u8 x, u8 y, u8 c, u8 fg, u8 bg)
{
    if(check_input(x,y,bg,fg))
    {
        return;
    }

    put_entry_text_buff(bg, fg, c, x, y);
}

u8 vga_textmode_getc(u8 x, u8 y)
{
    if(check_input(x,y,0,0))
    {
        return 0x0;
    }

    u16* entry = get_loc_text_buffer(x, y);
    return (*entry) & 0xff;
}

u8 vga_textmode_get_fg(u8 x, u8 y)
{
    if(check_input(x,y,0,0))
    {
        return 0x0;
    }

    u16* entry = get_loc_text_buffer(x, y);
    return ((*entry) >> VGA_FG_SHIFT) & 0xf;
}

u8 vga_textmode_get_bg(u8 x, u8 y)
{
    if(check_input(x,y,0,0))
    {
        return 0x0;
    }

    u16* entry = get_loc_text_buffer(x, y);
    return ((*entry) >> VGA_BG_SHIFT) & 0xf;
}

void vga_init()
{
    // Maybe verify that the VGA driver is set to fucking text mode?
    // Maybe allow input to allow different color modes? actually set pixels
}