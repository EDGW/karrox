#include "print.h"
#include "interrupt.h"
#include "io.h"
namespace arch_x86{

    /*
    send EOI to PIC and the interrupt ends
    defined in "interrupt.h"
    */
    void call_end_of_intr(){
        io::io_outb(X8259A_MASTER_1_PORT,EOI);
        io::io_outb(X8259A_SLAVE_1_PORT,EOI);
    }
    void (*intr_handlers[INTR_CNT])(uint32_t vecid,uint32_t error_code);    //registered intr handlers
    void (*default_intr_handler)(uint32_t vecid,uint32_t error_code);       //default intr handler
    void (*unknown_intr_handler)(uint32_t vecid,uint32_t error_code);       //unknown intr handler
    extern "C" void intr_c_handler(uint32_t vecid,uint32_t error_code){
        if(vecid>=INTR_CNT){
            unknown_intr_handler(vecid,error_code);
        }else if(intr_handlers[vecid]){
            intr_handlers[vecid](vecid,error_code);
        }else{
            default_intr_handler(vecid,error_code);
        }
    }
    void unknown_handler(uint32_t vecid,uint32_t error_code){
        boot::printf("Unknown interrupt %x occurred, errcode %x.\n",vecid,error_code);
    }

    void default_handler(uint32_t vecid,uint32_t error_code){
        call_end_of_intr();
    }

    /*initialize the intr handler*/
    void init_handlers(){
        default_intr_handler = default_handler;
        unknown_intr_handler = unknown_handler;
    }
}