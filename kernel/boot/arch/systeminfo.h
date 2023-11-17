/**
 * @file systeminfo.h
 * @brief The file defines an interface which contains a set of structs which describe the basic information about this computer and the system,
 * most commonly, the information is parsed from some bootloaders such as multiboot2 and be formatted like following structs.
 * any boots should implement this interface.
 * The file is referenced in @file arch.h
*/
#ifndef _ARCH_SYSTEMINFO_H
#define _ARCH_SYSTEMINFO_H
#include <stdint.h>
namespace arch{
    /**
     * @brief a struct describes the memory layout of this computer
    */
    struct memory_info{
        uint32_t mem_upper;/** the amount of the upper memory in kilobytes, the value of this field is the address of the first upper memory hole minus 1 megabyte*/
        uint32_t mem_lower;/** the amount of the lower 1MB memory in kilobytes*/
    };

    /**
     * @brief a struct describes the information of the device where we boot from
    */
    struct boot_device_info{
        uint32_t device_id;/** the BIOS drive number as understood by the BIOS INT 0x13 low-level disk interface. e.g.0x00 for the first floppy disk or 0x80 for the first hard disk. */
        uint32_t partition;/** the top-level partition number. Partition numbers are always start from 0.*/
        uint32_t sub_partition;/**a sub-partition in the top-level partition. Partition numbers are always start from 0.*/
    };
    
    /** 
     * @brief a struct describes the basic information of APM(Advanced Power Management) table,
     * for more information, reference to the documentation @ref http://www.microsoft.com/hwdev/busbios/amp_12.htm
     */
    struct apm_info{
        uint16_t verison;   /** version number */
        uint16_t cseg;      /** protected mode 32-bit code segment*/
        uint32_t offset;    /** offset of the entry point   */
        uint16_t cseg_16;   /** (not available) protected mode 16-bit code segment*/
        uint16_t dseg;      /** protected mode 16-bit data segment*/
        uint16_t flags;     /** APM flags*/
        uint16_t cseg_len;  /** the length of protected mode 32-bit code segment*/
        uint16_t cseg_16_len;/** (not available) the length of protected mode 16-bit code segment*/
        uint16_t dseg_len;  /** the length of protected mode 16-bit data segment*/
    };


    /**
     * @brief a struct describes the basic information of the system,
     * it's of large size and most commonly we use the pointer to the struct but itself as parameters
     * @warning Don't reference to this struct since it's not a release version
    */
    struct system_info{
            memory_info         mem_info;     /** memory layout info*/
            boot_device_info    boot_device;  /** boot device info*/
            apm_info            apm;          /** advanced power manager info*/
    };
}
#endif //_ARCH_SYSTEMINFO_H