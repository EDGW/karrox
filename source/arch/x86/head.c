#include "boot/multiboot2.h"
#include "stdint.h"
/**
 * @file head.c
 * @brief GNU GRUB Boot Loader Header File
*/


/**
 * '.multiboot' section is the first section present in the kernel
 * @brief header struct.
*/
__attribute__((section(".multiboot")))
struct multiboot_header header = {
    MULTIBOOT2_HEADER_MAGIC,    /** magic number    */
    MULTIBOOT_ARCHITECTURE_I386,/** x86 arch        */
    sizeof(struct multiboot_header),   /** header length   */
    - (MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (uint32_t)sizeof(struct multiboot_header)) /** checksum */
};