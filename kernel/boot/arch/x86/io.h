#ifndef _X86_IO_H
#define _X86_IO_H /*      A inline code set to provide some basic io(not disk io) operations       */
#include <stdint.h>

namespace arch_x86{

    
    //write a byte to specific port
    static inline void io_outb(uint16_t port, uint8_t data) {
        /*
        "a" use register al/ax/eax
        "N" 0-255 data context
        "d" use register dx
        */
        asm volatile ( "outb %b0, %w1" : : "a" (data), "Nd" (port));  
    }

    //write 'word_cnt' bytes from addr to specific port
    static inline void io_outsw(uint16_t port, const void* addr, uint32_t word_cnt) {
        /*
        "+" both input/output
        */
    asm volatile ("cld; rep outsw" : "+S" (addr), "+c" (word_cnt) : "d" (port));
    }

    //read a byte from specific port
    static inline uint8_t io_inb(uint16_t port) {
    uint8_t data;
    asm volatile ("inb %w1, %b0" : "=a" (data) : "Nd" (port));
    return data;
    }

    //read bytes from specific port to specific address
    static inline void io_insw(uint16_t port, void* addr, uint32_t word_cnt) {
    asm volatile ("cld; rep insw" : "+D" (addr), "+c" (word_cnt) : "d" (port) : "memory");
    }

    #endif
}