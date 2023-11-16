#include <stdint.h>

namespace io{

    
    //write a byte to specific port
     void io_outb(uint16_t port, uint8_t data) {
        /*
        "a" use register al/ax/eax
        "N" 0-255 data context
        "d" use register dx
        */
        asm volatile ( "outb %b0, %w1" : : "a" (data), "d" (port));  
    }
    //write a unsigned interger to specific port
     void io_outl(uint16_t port, uint32_t data) {
        /*
        "a" use register al/ax/eax
        "i" interger data context
        "d" use register dx
        */
        asm volatile ( "outl %0, %w1" : : "a" (data), "d" (port));  
    }
    //write 'word_cnt' bytes from addr to specific port
     void io_outsw(uint16_t port, const void* addr, uint32_t word_cnt) {
        /*
        "+" both input/output
        */
        asm volatile ("cld; rep outsw" : "+S" (addr), "+c" (word_cnt) : "d" (port));
    }

    //read a byte from specific port
     uint8_t io_inb(uint16_t port) {
        uint8_t data;
        asm volatile ("inb %w1, %b0" : "=a" (data) : "d" (port));
        return data;
    }
    //read a unsigned interger from specific port
     uint32_t io_inl(uint16_t port) {
        uint32_t data;
        asm volatile ("inl %w1, %0" : "=a" (data) : "d" (port));
        return data;
    }
    //read bytes from specific port to specific address
     void io_insw(uint16_t port, void* addr, uint32_t word_cnt) {
        asm volatile ("cld; rep insw" : "+D" (addr), "+c" (word_cnt) : "d" (port) : "memory");
    }

}