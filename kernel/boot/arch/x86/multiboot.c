#include "multiboot2.h"
__attribute__((section(".multiboot"))) 
struct multiboot_header header =
{
    MULTIBOOT2_HEADER_MAGIC,
    MULTIBOOT_ARCHITECTURE_I386,
    sizeof(header),
    -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + sizeof(header))
};