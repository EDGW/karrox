#ifndef _X86_PAGING_H
#define _X86_PAGING_H
#include "memory.h"

#define BOOT_KERNEL_PT_POS      0x300000    //[3MiB,4MiB) is preboot page table
#define PHYSICAL_KERNEL_PT_POS  0x400000    //[4MiB,12MiB) is kernel page table
#define KERNEL_PT_POS           (KERNEL_MEM_START_ADDR + PHYSICAL_KERNEL_PT_POS)

#define PG_FLAG_P       0b00000001  // Present
#define PG_FLAG_RW_RW   0b00000010
#define PG_FLAG_RW_R    0b00000000 
#define PG_FLAG_US_U    0b00000100  // determines it's a user page with no accessing limit
#define PG_FLAG_US_S    0b00000000  // determines it's a system page and can only be accessed by the supervisor
#define PG_FLAG_GLOBAL 0b100000000  // determines it is a global page

#ifndef ASM_FILE
    /**
     * @brief initialize basic paging layout
    */ 
    void init_paging();
#endif //ndef ASM


#endif //_X86_PAGING_H