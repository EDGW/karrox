#ifndef INTERRUPT_H_
#define INTERRUPT_H_
#include<stdint.h>
#include "interrupt_asm.h"
extern "C" uint32_t* intr_handler_table;
namespace arch_x86{
    /*      Interrupts       */
    struct intr_desc_entry{
        uint16_t offset_low;
        uint16_t seg_selector;
        uint8_t reserved = 0;
        uint8_t flag;
        uint16_t offset_high;
    };
    #define USER_INTR_ST            0x20
    #define INTR_CNT                0x30
    #define INTR_FLAG_P             0b10000000
    #define INTR_FLAG_DPL_0         0b00000000
    #define INTR_FLAG_DPL_3         0b01100000
    #define INTR_FLAG_TYPE_TASK     0x5
    #define INTR_FLAG_TYPE_INTR16   0x6
    #define INTR_FLAG_TYPE_INTR32   0xe
    #define INTR_FLAG_TYPE_TRAP16   0x7
    #define INTR_FLAG_TYPE_TRAP32   0xf
    #define INTR_FLAG_DEFAULT_DPL0  (INTR_FLAG_P | INTR_FLAG_DPL_0 | INTR_FLAG_TYPE_INTR32)
}
#endif