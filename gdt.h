#ifndef MEME_GDT
#define MEME_GDT

// The Global descriptor table (GDT) was created at first to expand the amount
// of memory you could access using 16 bits. It was extended to add memory
// protection. Now in days it exsist mainly for backwards compatibilty. We will
// use a "flat" memory model where the kernel and user code and data segments
// simply cover the entire 4Gb address space and this mechanism is pretty much
// by passed.

// The GDT defines memory regions along with permisions adn extra meta data
// describing how this segment is used. The registers CS,DS, and SS to keep 
// indexes (offsets) into the GDT. You use the gdt entry format to store 
// entires in this table, and use the lgdt, sgdt to set and get the tables 
// location. 

#include "types.h"

#define GDT_NULL_SECTOR 0x0
#define KERNEL_CODE_SECTOR 0x8
#define KERNEL_DATA_SECTOR 0x10
#define USER_CODE_SECTOR 0x8
#define USER_DATA_SECTOR 0x10

///////////////////////////////////////////////////////////
// GDT entry access parameter bit fields
///////////////////////////////////////////////////////////

// Access Byte [7] - Present bit. 
#define GDT_SEGMENT_PRESENT_SHIFT 7
#define GDT_SEGMENT_PRESENT       0x1
#define GDT_SEGMENT_NOT_PRESENT   0x0

// Access Byte [5-6] - Privilege level.
#define GDT_DPL_SHIFT             5
#define GDT_DPL_KERNEL            0x0
#define GDT_DPL_USER              0x3

// Descriptor type [4] - System or Code/Data segment
#define GDT_DESC_TYPE_SHIFT       4
#define GDT_DESC_TYPE_SYS         0x0
#define GDT_DESC_TYPE_NON_SYS     0x1

// Executable bit [3] - Is segment executable
#define GDT_EXE_SHIFT            3
#define GDT_IS_EXE               0x1
#define GDT_NOT_EXE              0x0

// Direction bit/Conforming bit [2]
// Data selectors: 0) the segment grows up. 1) the segment grows down
// Code selectors: 0) code in this segment can only be executed from the ring set in Privl.
//                 1) code in this segment can be executed from an equal or lower privilege level
#define GDT_DIRECTION_SHIFT      2
#define GDT_CONFORMING_SHIFT     2
#define GDT_DIRECTION_UP         0x0
#define GDT_DIRECTION_DOWN       0x1
#define GDT_CONFORMING_NO        0x0
#define GDT_CONFORMING_YES       0x1

// R/W bit [1]
// Data selectors: 0) No write. 1) Can Write. Read always allowed
// Code selectors: 0) No read.  1) Can read.  Write never allowed
#define GDT_RW_SHIFT             1
#define GDT_SET_RW               0x1
#define GDT_NO_RW                0x0

// Accessed Bit [0] - cpu sets this
#define GDT_ACCESSED_SHIFT       0
#define GDT_ACCESSED             0x1
#define GDT_NOT_ACCESSED         0x0

///////////////////////////////////////////////////////////
// GDT entry granularity parameter bit fields
///////////////////////////////////////////////////////////

// Granularity Bit [7] - scaling factor for limit value
#define GDT_GRANULARITY_SHIFT    7
#define GDT_4K_GRAN              0x1
#define GDT_1b_GRAN              0x0

// Operand size [6]
#define GDT_OP_SIZE_SHIFT        6
#define GDT_16b_OP_SIZE          0x0
#define GDT_32b_OP_SIZE          0x1

// Bits 5-6 always 0

// Limit upper 4 bits
#define GDT_LIMIT_UPPER_SHFT     0

// A GDT entry defines a memory segment w/ the following fields
//    limit_low   - Lower 16 bits giving size of mem region
//    base_low    - lower 16 bits giving starting addr of mem region
//    base_middle - middle 8 bits of base addr
//    access      - see above
//    granularity - see above
//    base_high   - upper 8 bits of memory address
struct gdt_entry
{
    u16 limit_low;
    u16 base_low;
    u8 base_middle;
    u8 access;
    u8 granularity;
    u8 base_high;
} __attribute__((packed));

// A GDT pointer. This is the data structure that is used by the lgdt and sgdt
// instructions to register / return the location of the GDT in memory.
struct gdt_ptr
{
    u16 size;
    u32 base;
} __attribute__((packed));

// Create the flat memory model, register the new table w/ the processor, and
// finally update the segment registers, completing the updating of the GDT.
void gdt_install();

// Simply return where the current GDT is pointing. 
void gdt_get_curr_gdt_ptr(struct gdt_ptr *p);

#endif