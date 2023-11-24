#ifndef _X86_BOOTINFO_H
#define _X86_BOOTINFO_H
#include "stdint.h"
#ifndef ASM_FILE

/**
 * @brief the information about the memory of this PC
*/
struct memory_info{
    uint32_t lower_mem_size;/** The size of low physical memory in kilobytes. */
    uint32_t upper_mem_size;/** The size of upper physical memory in kilobytes. */
};

/**
 * @brief the basic information about this PC
 * @warning use the pointer to this struct but itself because of the too-large size
*/
struct bootinfo{
    struct memory_info memory;
};

/**
 * @brief get a pointer to the current bootinfo instance
*/
struct bootinfo* get_bootinfo();
#endif //ASM_FILE
#endif //_X86_BOOTINFO_H