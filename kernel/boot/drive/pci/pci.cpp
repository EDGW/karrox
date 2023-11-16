#include<stdint.h>
#include "debug.h"
#include "print.h"
#include "arch/io.h"
#include "pci.h"
using namespace io;
namespace drive::pci{
    #ifdef DEBUG
    void debug(){
        boot::printf("---- PCI debugging line ----\n");
        for(uint8_t d = 0;d<255u;d++)for(uint8_t i = 0;i<255u;i++){
            int vid = pci_config_read_word(d,i,0,VENDOR_ID_RED);
            uint8_t did = pci_config_read_byte(d,i,0,CLASS_CODE_REG);
            uint8_t did2 = pci_config_read_byte(d,i,0,SUBCLASS_REG);
            uint8_t did3 = pci_config_read_byte(d,i,0,PROG_IF_REG);
            if(vid!=0xffffu)boot::printf("PCI#%d Slot#%d: VENDOR ID %x, CLASSCODE %x, SUBC %x, PI %x\n",d,i,vid,did,did2,did3);
        }
    }
    #endif //DEBUG

    
    /*
    read a byte from pci configuration space
    bus:    the bus id of the device
    slot:   the device is in the bus
    func:   function id
    offset: the size of config space is 256bytes, and this command is used to read 2 bytes from it
            the variable determines the offset in the config space
            the value must be aligned in 4 bytes
    */
    uint8_t pci_config_read_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
        uint16_t val = pci_config_read_word(bus,slot,func,offset);
        return (uint16_t)((val >> ((offset & 1) * 8))&0xff);
    } 
    /*
    /*
    read a word from pci configuration space
    bus:    the bus id of the device
    slot:   the device is in the bus
    func:   function id
    offset: the size of config space is 256bytes, and this command is used to read 2 bytes from it
            the variable determines the offset in the config space
            the value must be aligned in 4 bytes
    */
    uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
        uint32_t val = pci_config_read_long(bus,slot,func,offset);
        return (uint16_t)((val >> ((offset & 2) * 8))&0xffff);
    } 
    /*
    read a interger from pci configuration space
    bus:    the bus id of the device
    slot:   the device is in the bus
    func:   function id
    offset: the size of config space is 256bytes, and this command is used to read 2 bytes from it
            the variable determines the offset in the config space
            the value must be aligned in 4 bytes
    */
    uint32_t pci_config_read_long(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
        uint32_t address;
        uint32_t lbus  = (uint32_t)bus;
        uint32_t lslot = (uint32_t)slot;
        uint32_t lfunc = (uint32_t)func;
    
        address = (uint32_t)((lbus << 16) | (lslot << 11) |
                (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    
        // Write out the address
        io_outl(PCI_CONFIG_ADDRESS, address);
        // Read in the data
        // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
        return (io_inl(PCI_CONFIG_DATA));
    }
}
