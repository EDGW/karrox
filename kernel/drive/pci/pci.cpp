#include <stdint.h>
#include "lib/debug.h"
#include "lib/print.h"
#include "lib/io.h"
#include "pci.h"
namespace drive::pci{

    /*  as name presents    */
    uint16_t pci_device_count;

    /*  store the currently scanned devices     */
    pci_device_info pci_devices[MAX_DEVICE_COUNT];

    /*  scan all the devices    */
    void rescan_all_devices(){
        pci_device_count = 0;
        uint16_t count = 0;
        for(uint8_t bus = 0;bus<255;bus++)for(uint8_t slot = 0;slot<255;slot++){
            pci_device_info info =  get_device_info(bus,slot);
            if(info.vendor_id==INVALID_VENDOR_ID)continue;
            pci_devices[count]=info;
            count++;
        }
        pci_device_count = count;
    }

    pci_device_info get_device_info(uint8_t bus, uint8_t slot){
        uint16_t vendor = pci_config_read_word(bus,slot,0,VENDOR_ID_REG);
        if(vendor==INVALID_VENDOR_ID) return {bus,slot,INVALID_VENDOR_ID};//invalid device return a info struct with invalid vendor id
        uint16_t device = pci_config_read_word(bus,slot,0,DEVICE_ID_REG);
        uint16_t classids = pci_config_read_word(bus,slot,0,CLASS_CODE_REG);//both class code & subclass id
        uint8_t subclass = (uint8_t)(classids&0xff);//low 8 bit
        uint8_t classcode = (uint8_t)((classids&0xff00)>>8);//high 8 bit
        uint8_t proginterf = pci_config_read_byte(bus,slot,0,PROG_IF_REG);
        return {bus,slot,vendor,device,classcode,subclass,proginterf};
    }

    uint8_t pci_config_read_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
        uint16_t val = pci_config_read_word(bus,slot,func,offset);
        return (uint16_t)((val >> ((offset & 1) * 8))&0xff);
    } 

    uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
        uint32_t val = pci_config_read_long(bus,slot,func,offset);
        return (uint16_t)((val >> ((offset & 2) * 8))&0xffff);
    } 

    uint32_t pci_config_read_long(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
        uint32_t address;
        uint32_t lbus  = (uint32_t)bus;
        uint32_t lslot = (uint32_t)slot;
        uint32_t lfunc = (uint32_t)func;
    
        address = (uint32_t)((lbus << 16) | (lslot << 11) |
                (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
    
        // Write out the address
        outl(PCI_CONFIG_ADDRESS, address);
        // Read in the data
        // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
        return (inl(PCI_CONFIG_DATA));
    }

    void init(){
        boot::printf("initialize pci drive\n");
        rescan_all_devices();
    }


    #ifdef DEBUG
    void debug(){
        boot::printf("---- PCI debugging line ----\n");
        boot::printf("PCI Devices:\n");
        for(uint16_t i = 0;i<pci_device_count;i++){
            boot::printf("#%d\t PCI#%d\t SLOT#%d\t:VDR %x\t,DEV %x\t,CLS %x\t,SCLS %x\t,PGIF %x\n",i,
            pci_devices[i].bus,
            pci_devices[i].slot,
            pci_devices[i].vendor_id,
            pci_devices[i].device_id,
            pci_devices[i].class_code,
            pci_devices[i].subclass_code,
            pci_devices[i].prog_interface_code);
        }
    }
    #endif //DEBUG

}
