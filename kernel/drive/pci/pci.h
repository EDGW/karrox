#ifndef _DRIVE_PCI_PCI_H
#define _DRIVE_PCI_PCI_H
#include<stdint.h>
#include "lib/debug.h"
namespace drive::pci{


    
    #ifdef DEBUG
    void debug();
    #endif //DEBUG

    /*      Configuration Space         
            0       8|9      15|16      23|24        31
    0x00        Vendor ID      |       Device ID            [READONLY]VENDOR ID is the name of the producer of the device of which DEVICE ID is the name
    0x04        Command        |       Status               Command provides control over a device's ability to generate and respond to PCI cycles. 
                                                            Where the only functionality guaranteed to be supported by all devices is, 
                                                            when a 0 is written to this register, the device is disconnected from the PCI bus for all accesses except Configuration Space access. 
                                                            Status is a register used to record status information for PCI bus related events. 
    0x08    Revision | Prog IF | Subclass | Class Code      [READONLY]Revision ID records the version of PCIE while Class Code records it's category
                                                            [READONLY]Class Code is a read-only register that specifies the type of function the device performs. 
                                                            [READONLY]Subclass is a read-only register that specifies the specific function the device performs. 
                                                            [READONLY]Prog IF(Programming Interface Byte) is a read-only register that specifies a register-level programming interface the device has, if it has any at all. 
    0x0c    Cacheline| Latency |  Header  |     BIST        [READONLY]Header Type: bits 7 determines whether this device is multifunctional or of a single function
               Size  |  Timer  |   Type   |                                        bits 0-6 determines the type of this configuration space
                                                            BIST represents that status and allows control of a devices BIST (built-in self test). 
                                                            Latency Timer specifies the latency timer in units of PCI bus clocks. 
                                                            Cacheline Size specifies the system cache line size in 32-bit units. A device can limit the number of cacheline sizes it can support, 
                                                            if a unsupported value is written to this field, the device will behave as if a value of 0 was written. 
    0x10           Base Address Register 0
    0x14           Base Address Register 1                  BAR stores the base addr of the space this PCI device uses after reseted
    0x18           Base Address Register 2                  pay attention that the addr is PCI bus addr but memory offset
    0x1c           Base Address Register 3                  every device can owns no more than 6 base addr spaces
    0x20           Base Address Register 4                  but few devices use all of the six spaces
    0x24           Base Address Register 5                  A practical way to get the length of the space this device used is to read from the reg after write 0xffffffff
    0x28       0x28-0x2b(4 bytes) Cardbus CIS Pointer
    0x2c   Subsystem Vendor ID |     Subsystem ID           [READONLY] same as Vendor ID & Device ID
    0x30  0x30-0x33(4 bytes) Expansion ROM Base Address     [READONLY] base addr of expansion rom
    0x34              Reserved            | Cap Pointer     
    0x38                   Reserved
    0x3c     Max_Lat | Min_Gnt | INTR Pin | INTR Line       INTR Line is set by sys, determines the intr vec number (unused)
    
    */
    #define PCI_CONFIGURATION_SPACE_LAYOUT
    #define PCI_HEADER_GENERAL_DEVICE       0x0
    #define PCI_HEADER_PCI_BRIDGE           0x1
    #define PCI_HEADER_PCI_CARDBUS_BRIDGE   0x2

    #define VENDOR_ID_RED                   0x00
    #define DEVICE_ID_REG                   0x02
    #define COMMAND_REG                     0x4
    #define STATUS_REG                      0x6
    #define REVISION_REG                    0x08
    #define PROG_IF_REG                     0x09
    #define SUBCLASS_REG                    0x0a
    #define CLASS_CODE_REG                  0x0b
    #define CACHELINE_SIZE_REG              0x0c
    #define LATENCY_TIMER_REG               0x0d
    #define HEADER_TYPE_REG                 0x0e
    #define BIST_REG                        0x0f
    #define BAR0_REG                        0x10
    #define BAR1_REG                        0x14
    #define BAR2_REG                        0x18
    #define BAR3_REG                        0x1c
    #define BAR4_REG                        0x20
    #define BAR5_REG                        0x24
    #define CARDBUS_CIS_PTR_REG             0x28
    #define SUBS_VENDOR_ID_REG              0x2c
    #define SUBS_ID_REG                     0x2e
    #define EXPAN_ROM_BASE_ADDR_REG         0x30
    #define CAP_PTR_REG                     0x37
    #define MAX_LAT_REG                     0x3c
    #define MIN_GNT_REG                     0x3d
    #define INTR_PIN_REG                    0x3e
    #define INTR_LINE_REG                   0x3f

    #define PCI_CONFIG_ADDRESS              0xCF8
    #define PCI_CONFIG_DATA                 0xCFC


    uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
    uint32_t pci_config_read_long(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
    uint8_t pci_config_read_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
}


#endif //_DRIVE_PCI_PCI_H