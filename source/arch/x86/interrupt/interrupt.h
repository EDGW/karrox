#ifndef _X86_INTERRUPT_H
#define _X86_INTERRUPT_H
#include "stdint.h"

#define INTR_CNT 0x30 

#define INTR_DESC_FLAG_TYPE_TASK    0x5         // determines it's a task gate
#define INTR_DESC_FLAG_TYPE_INTR16  0x6         // determines it's a 16-bit intrrupt gate
#define INTR_DESC_FLAG_TYPE_TRAP16  0x7         // determines it's a 16-bit trap gate
#define INTR_DESC_FLAG_TYPE_INTR32  0xe         // determines it's a 32-bit intrrupt gate
#define INTR_DESC_FLAG_TYPE_TRAP32  0xf         // determines it's a 32-bit trap gate
#define INTR_DESC_FLAG_DPL0         0b00000000   // determines this gate can only be called by INT instruction by the supervisor
#define INTR_DESC_FLAG_DPL3         0b01100000   // determines this gate can only be called by INT instruction by anyone
#define INTR_DESC_FLAG_P            0b10000000  // Presents


#ifndef ASM_FILE

    /**
     * @brief original(assembly) interrupt handlers
     * @details defined in 'intr_handler.S.gen.py'
    */
    extern void *intr_original_handlers[INTR_CNT];

    /**
     * @brief initialize basic interrupt manager
    */ 
    void init_intr();
    
    /**
     * @brief enable the interrupts
    */
    void intr_turn_on();

    /**
     * @brief disable the interrupts
    */
    void intr_turn_off();

    /**
     * @brief interrupt gate descriptor
    */
    struct intr_descriptor
    {
        uint16_t low_offset;
        uint16_t segment_selector;
        uint8_t reserved;
        uint8_t flags;
        uint16_t high_offset;
    };

    /**
     * @brief quickly create a interrupt descriptor
     * @param offset        the offset of the interrupt handler
     * @param segm_selector the selector of the interrupt handler
     * @param flags         the flags of the interrupt gate
    */
    static inline struct intr_descriptor create_intr_desc(uint32_t offset,uint16_t segm_selector,uint8_t flags){
        struct intr_descriptor desc;
        desc.low_offset = (uint16_t)(offset & 0xffff);
        desc.high_offset = (uint16_t)((offset & 0xffff0000)>>16);
        desc.reserved=0;
        desc.flags=flags;
        desc.segment_selector =segm_selector;
        return desc;
    }
    
#endif //ASM_FILE

#endif //_X86_INTERRUPT_H