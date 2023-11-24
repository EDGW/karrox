/**
 * @file interrupt.c
 * @brief basic init and management functions of interrupt
*/
#include "interrupt.h"
#include "stdint.h"
#include "pic.h"
#include "../memory/memory.h"

/**
 * @brief a table of interrupt handler indexed by interrupt verctor number
*/
void* intr_handler_table[INTR_CNT];

/**
 * @brief Interrupt Descriptor Table(IDT)
*/
struct intr_descriptor idt[INTR_CNT];

int num = 0;
/**
 * @brief default interrupt handler
 * @param intcode the vector number of the interrupt
 * @param errcode the error code (or 0) presented by the interrupt
*/
void default_intr_handler(uint32_t intcode,uint32_t errcode){
    call_end_of_intr();
}

/**
 * @brief init interrupt handlers
*/
static inline void init_intr_handler_table(){
    for(int i = 0;i<INTR_CNT;i++){
        idt[i]=create_intr_desc((uint32_t)intr_original_handlers[i],GDT_SELECTOR_KERNEL_CODE,INTR_DESC_FLAG_TYPE_INTR32 | INTR_DESC_FLAG_DPL0 | INTR_DESC_FLAG_P);
        intr_handler_table[i]=(void*)default_intr_handler;
    }
    uint64_t idtr = ((uint64_t)(uint32_t)(&idt))<<16;
    idtr |= sizeof(idt)-1;
    asm volatile(
        "lidt %0\n"
        ::"m"(idtr));
}

void intr_turn_on(){//extern "interrupt.h"
    asm volatile("sti");
}

void intr_turn_off(){//extern "interrupt.h"
    asm volatile("cli");
}

void init_intr(){//extern "interrupt.h"
    init_intr_handler_table();
    init_pic();
    intr_turn_on();
}