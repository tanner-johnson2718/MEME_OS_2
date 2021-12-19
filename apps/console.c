#include "apps/console.h"
#include "core/types.h"
#include "drivers/vga.h"

// Internal State
u8 cur_x = 0;
u8 cuy_y = 0;
u8 cur_bg = VGA_BLACK;
u8 cur_fg = VGA_WHITE;

void console()
{
    // void vga_textmode_putc(u8 x, u8 y, u8 c, u8 fg, u8 bg);
    while(1){};
}