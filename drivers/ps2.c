#include "drivers/ps2.h"
#include "core/types.h"
#include "core/io_port.h"
#include "drivers/serial.h"
#include "core/irq.h"

// ASCII map to trasnlate keys to ascii text
u8 key_ascii_map[128] = 
{
    0,  27,                                 // escape
    '1', '2', '3', '4', '5', '6', '7', '8',	// 9 
    '9', '0', '-', '=', '\b',	            // Backspace
    '\t',			                        // Tab 
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 
    'o', 'p', '[', ']', '\n',	            // Enter key
    0,			/* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
    '\'', '`',   0,		/* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
    'm', ',', '.', '/',   0,				/* Right shift */
    '*',
    0,	/* Alt */
    ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
    '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

// Keyboard state
struct keyboard_state state = {0};

void update_caps_state()
{
    u8 shift_held = (state.state >> PS2_KEYBOARD_STATE_SHIFT_HELD_SHIFT) & 0x1;
    u8 caps_on = (state.state >> PS2_KEYBOARD_STATE_CAPSL_SHIFT) & 0x1;
    u8 caps = caps_on ^ shift_held;

    if(caps){
        state.state |=  (0x1 << PS2_KEYBOARD_STATE_OUT_CAPS);
    }
    else
    {
        state.state &= PS2_KEYBOARD_STATE_CAPS_OFF_MASK;
    }
    
}

u8 is_alphabet_char(u8 in_ascii)
{
    if(in_ascii >= 0x61 && in_ascii <= 0x7A)
    {
        return 1;
    }

    return 0;
}

void ps2_keyboard_irq(void)
{
    u8 in = inb(PS2_DATA_PORT);

    // Check if key released
    if(in & PS2_KEYBOARD_RELEASE_MASK)
    {
        // key released event

        // Check if shift released
        if(in == PS2_KEYBOARD_LSHIFT_RELES || in == PS2_KEYBOARD_RSHIFT_RELES)
        {
            // update shfit held bit in state
            state.state &= PS2_KEYBOARD_STATE_SHIFT_RELE_MASK;
        }

        update_caps_state();
    }
    else
    {
        // key pressed event

        // Check if shift pressed
        if(in == PS2_KEYBOARD_LSHIFT_PRESS || in == PS2_KEYBOARD_RSHIFT_PRESS)
        {
            // update shfit held bit in state
            state.state |= (0x1 << PS2_KEYBOARD_STATE_SHIFT_HELD_SHIFT);
        }

        // check if caps lock pressed
        if(in == PS2_KEYBOARD_CAPSLOCK)
        {
            u8 caps_on = (state.state >> PS2_KEYBOARD_STATE_CAPSL_SHIFT) & 0x1;

            if(!caps_on){
                state.state |=  (0x1 << PS2_KEYBOARD_STATE_CAPSL_SHIFT);
            }
            else
            {
                state.state &= PS2_KEYBOARD_STATE_SHIFT_CAPSL_MASK;
            }
        }

        update_caps_state();

        // map input to actual ascii and output
        u8 in_ascii = key_ascii_map[in];

        // output caps if caps bit set in state and output char is a-z
        if(( (state.state >> PS2_KEYBOARD_STATE_OUT_CAPS) & 0x1) && is_alphabet_char(in_ascii))
        {
            in_ascii -= 0x20;
        }

        serial_putc(in_ascii);
    }
    
}

void ps2_init(void)
{
    irq_register_PIC_handler(ps2_keyboard_irq, IRQ_PIC_KEY);
}