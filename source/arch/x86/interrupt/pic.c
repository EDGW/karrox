/**
 * @file pic.c
 * @brief basic init and management functions of programmable interrupt controller
*/
#include "pic.h"
#include "kernel/io.h"

uint8_t master_mask_mode = 0;    //block all interrupts
uint8_t slave_mask_mode  = 0;    //block all interrupts

void intr_master_block(uint8_t filter){    //extern "pic.h"
    master_mask_mode |= filter;
    kernel_io_outb(X8259A_MASTER_2_PORT,master_mask_mode);
}

void intr_slave_block(uint8_t filter){    //extern "pic.h"
    slave_mask_mode |= filter;
    kernel_io_outb(X8259A_MASTER_2_PORT,slave_mask_mode);
}

void intr_master_unblock(uint8_t filter){    //extern "pic.h"
    master_mask_mode &= (~filter);
    kernel_io_outb(X8259A_MASTER_2_PORT,master_mask_mode);
}

void intr_slave_unblock(uint8_t filter){    //extern "pic.h"
    slave_mask_mode &= (~filter);
    kernel_io_outb(X8259A_MASTER_2_PORT,slave_mask_mode);
}

void call_end_of_intr(){    //extern "pic.h"
    kernel_io_outb(X8259A_MASTER_1_PORT,OCW2_END_OF_INTR);
}

void init_pic(){    //extern "pic.h"
    /* initialize master    */
    kernel_io_outb(X8259A_MASTER_1_PORT,ICW1 | ICW1_MODE_CASCADE | ICW1_TRIGGER_EDGE | ICW1_ICW4_ENABLED); //ICW1
    kernel_io_outb(X8259A_MASTER_2_PORT,0x20);  // ICW2 intr vector id starts from 0x20 , IR[0-7] is 0x20 - 0x27
    kernel_io_outb(X8259A_MASTER_2_PORT,0b00000100);  // ICW 3 IR2 connects the slave
    kernel_io_outb(X8259A_MASTER_2_PORT,ICW4 | ICW4_PROCESSOR_X86);  // ICW 4
    /* initialize slave    */
    kernel_io_outb(X8259A_SLAVE_1_PORT,ICW1 | ICW1_MODE_CASCADE | ICW1_TRIGGER_EDGE | ICW1_ICW4_ENABLED); //ICW1
    kernel_io_outb(X8259A_SLAVE_2_PORT,0x28);  // ICW2 intr vector id starts from 0x28 , IR[8-15] is 0x28 - 0x2f
    kernel_io_outb(X8259A_SLAVE_2_PORT,2);     // ICW 3 IR2 connects the slave
    kernel_io_outb(X8259A_SLAVE_2_PORT,ICW4 | ICW4_PROCESSOR_X86);  // ICW 4
    master_mask_mode = slave_mask_mode = 0; //init mask mode, unblock all
    /*intr_master_block(0b11111110);
    intr_slave_block(0b11111111);       //block all interrupts except clock intr.*/
}