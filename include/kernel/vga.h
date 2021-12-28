#ifndef MEME_VGA
#define MEME_VGA

#include "types.h"

// ?? 
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
#define VGA_BUFFER_WIDTH  80
#define VGA_BUFFER_HEIGHT 25

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

// VGA ERROR Codes
#define VGA_OUT_BOUNDS_INDICES 0x1
#define VGA_INVALID_COLOR      0x2

void vga_init();
void vga_textmode_putc(u8 x, u8 y, u8 c, u8 fg, u8 bg);
u8 vga_textmode_getc(u8 x, u8 y);
u8 vga_textmode_get_fg(u8 x, u8 y);
u8 vga_textmode_get_bg(u8 x, u8 y);

#endif