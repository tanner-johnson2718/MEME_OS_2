#include "pci.h"
#include "types.h"
#include "io_port.h"

///////////////////////////////////////////////////////////////////////////////
// Private defines, macros, and registers
///////////////////////////////////////////////////////////////////////////////

// Registers for accessing the configuration space, write address to addr port
// and read from the data port. On failure,returns 0xFFFF in the vendor ID.
#define PCI_CONFIG_ADDR_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC