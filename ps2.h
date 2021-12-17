#ifndef MEME_PS2
#define MEME_PS2

#define PS2_DATA_PORT 0x60
#define PS2_STAT_PORT 0x64   // read
#define PS2_CMND_PORT 0x64   // write

// Bit map breakout for stat register
#define PS2_OUT_BUFF_SHIFT 0x0
#define PS2_OUT_BUFF_EMPTY 0x0
#define PS2_OUT_BUFF_FULL  0x1

#define PS2_IN_BUFF_SHIFT 0x1
#define PS2_IN_BUFF_EMPTY 0x0
#define PS2_IN_BUFF_FULL  0x1

#define PS2_SYS_FLAG_SHIFT 0x2    // for bios

#define PS2_CMD_SHIFT  0x3
#define PS2_CMD_DEVICE 0x0
#define PS2_CMD_CTLR   0x1

#define PS2_TIMEOUT_ERROR 0x6
#define PS2_TIMEOUT       0x1
#define PS2_NO_TIMEMOUT   0x0

#define PS2_PARITY_ERROR_SHIFT 0x7
#define PS2_PARITY_ERROR       0x1
#define PS2_PARITY_NO_ERROR    0x0

// init the ps2 module and register device handlers
void ps2_init(void);

#endif