#ifndef INTERRUPT_ASM_H
#define INTERRUPT_ASM_H

#define X8259A_MASTER_1_PORT    0x20    //master 1 port of 8259A,ICW 1 OCW2 & OCW3 
#define X8259A_MASTER_2_PORT    0x21    //master 2 port of 8259A,ICW 2 ICW3 ICW3 & OCW1
#define X8259A_SLAVE_1_PORT     0xA0    //slave  1 port of 8259A,ICW 1 OCW2 & OCW3 
#define X8259A_SLAVE_2_PORT     0xA1    //slave  2 port of 8259A,ICW 2 ICW3 ICW3 & OCW1

#define ICW1        0b00010000          //base value
#define ICW1_IC4    0b00000001          //need icw4
#define ICW1_SNGL   0b00000010          //single or cascade
#define ICW1_LTIM_LEVEL 0b00001000      //level trigger
#define ICW1_LTIM_EDGE  0b00000000      //edge trigger
#define EOI         0x20
#endif //INTERRUPT_ASM_H