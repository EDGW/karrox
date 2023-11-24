#ifndef _X86_PIC_H
#define _X86_PIC_H
#include "stdint.h"

#define X8259A_MASTER_1_PORT        0x20  /** the even port of the master chip of 8259A, ICW1,OCW2,OCW3 */
#define X8259A_MASTER_2_PORT        0x21  /** the odd port of the master chip of 8259A, ICW2,ICW3,ICW4,OCW1 */
#define X8259A_SLAVE_1_PORT         0xA0  /** the even port of the slave chip of 8259A, ICW1,OCW2,OCW3 */
#define X8259A_SLAVE_2_PORT         0xA1  /** the odd port of the slave chip of 8259A, ICW2,ICW3,ICW4,OCW1 */


/**
 * @brief the identifier of Initialization Command Words 1 register.
*/
#define ICW1                0x10
#define ICW1_MODE_SINGLE    0b00000010  // determines the 8259A chipset is working on single mode
#define ICW1_MODE_CASCADE   0b00000000  // determines the 8259A chipset is working on cascade mode, the slave chip was connected to the master to provide more interrupts.
#define ICW1_ICW4_ENABLED   0b00000001  // enable ICW4, this bit must be set on x86 processor system
#define ICW1_ICW4_DISABLED  0b00000000  // disable ICW4
#define ICW1_TRIGGER_LEVEL  0b00001000  // level trigger mode
#define ICW1_TRIGGER_EDGE   0b00000000  // edge trigger mode


/**
 * @brief the identifier of Initialization Command Words 4 register.
*/
#define ICW4                    0x0
#define ICW4_SPEC_FULLY_NESTED  0b00010000
#define ICW4_BUFFER_ENABLED     0b00001000  // enable buffer on interrupts.
#define ICW4_BUF_MASTER         0b00000100  // determines it's the master 8259A chip, working only when buffer is enabled
#define ICW4_BUF_SLAVE          0b00000000  // determines it's the slave 8259A chip, working only when buffer is enabled
#define ICW4_AUTO_EOI           0b00000010  // auto End Of Interrupt
#define ICW4_PROCESSOR_8080     0b00000000 
#define ICW4_PROCESSOR_X86      0b00000001

/**
 * @brief determines which interrupt should be masked.
*/
#define OCW1    0x0

/**
 * @brief interrupt operating register
*/
#define OCW2        0x0
#define OCW2_END_OF_INTR 0b00100000


#ifndef ASM_FILE

/**
 * @brief block from specific intr on the master chip
 * @param filter block filter, the bits which is set to 1 will be blocked
*/
void intr_master_block(uint8_t filter);

/**
 * @brief block from specific intr on the slave chip
 * @param filter block filter, the bits which is set to 1 will be blocked
*/
void intr_slave_block(uint8_t filter);

/**
 * @brief unblock specific intr on the master chip
 * @param filter block filter, the bits which is set to 1 will be unblocked
*/
void intr_master_unblock(uint8_t filter);

/**
 * @brief unblock specific intr on the slave chip
 * @param filter block filter, the bits which is set to 1 will be unblocked
*/
void intr_slave_unblock(uint8_t filter);

/**
 * @brief the EOI sign, was called by interrupt handler after having handled the interrupt.
*/
void call_end_of_intr();

/**
 * @brief initialize programmable interrupt controller
*/
void init_pic();
#endif //ASM_FILE

#endif //_X86_PIC_H