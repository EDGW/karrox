#ifndef _X86_MEMORY_H
#define _X86_MEMORY_H

namespace arch_x86{

    #define SELECTOR_KERNEL_DATA    0x18    /*  kernel data segment selector(GRUB's initial value)*/
    #define SELECTOR_KERNEL_CODE    0x10    /*  kernel code segment selector(GRUB's initial value)*/
    #define SELECTOR_USER_DATA      0x08    /*  user data segment selector*/
    #define SELECTOR_USER_CODE      0x00    /*  user code segment selector*/

    #define ACCESS_P                (((1LL<<7)<<8)<<32)
    #define ACCESS_DPL0             (((0LL<<5)<<8)<<32)
    #define ACCESS_DPL3             (((3LL<<5)<<8)<<32)
    #define ACCESS_S_TEXT           (((1LL<<4)<<8)<<32)
    #define ACCESS_S_SYS            (((0LL<<4)<<8)<<32)
    #define ACCESS_E_EXECUTABLE     (((1LL<<3)<<8)<<32)
    #define ACCESS_DIRECTION_UP     (((0LL<<2)<<8)<<32)
    #define ACCESS_DIRECTION_DOWN   (((1LL<<2)<<8)<<32)
    #define ACCESS_CONFORMING       (((1LL<<2)<<8)<<32)
    #define ACCESS_RW_CODE_E        (((0LL<<1)<<8)<<32)
    #define ACCESS_RW_CODE_ER       (((1LL<<1)<<8)<<32)
    #define ACCESS_RW_DATA_R        (((0LL<<1)<<8)<<32)
    #define ACCESS_RW_DATA_RW       (((1LL<<1)<<8)<<32)

    #define ACCESS_KERNEL           ACCESS_P | ACCESS_DPL0 | ACCESS_S_TEXT
    #define ACCESS_USER             ACCESS_P | ACCESS_DPL3 | ACCESS_S_TEXT

    #define ACCESS_DATA             (ACCESS_DIRECTION_UP | ACCESS_RW_DATA_RW)
    #define ACCESS_CODE             (ACCESS_E_EXECUTABLE | ACCESS_RW_CODE_ER)

    #define FLAG_DBL_16             ((0b00LL<<1)<<52)
    #define FLAG_DBL_32             ((0b10LL<<1)<<52)
    #define FLAG_DBL_64             ((0b01LL<<1)<<52)
    #define FLAG_G_1B               ((0b0LL<<3)<<52)
    #define FLAG_G_4K               ((0b1LL<<3)<<52)

    #define DEFAULT_FLAG            (FLAG_DBL_32 | FLAG_G_4K )
}

#endif //_X86_MEMORY_H