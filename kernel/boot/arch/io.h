#ifndef _ARCH_IO_H
#define _ARCH_IO_H /*      A inline code set to provide some basic io(not disk io) operations       */
#include <stdint.h>

namespace io{
    //write a byte to specific port
    void io_outb(uint16_t port, uint8_t data);
    //write a unsigned interger to specific port
    void io_outl(uint16_t port, uint32_t data);
    //write 'word_cnt' bytes from addr to specific port
    void io_outsw(uint16_t port, const void* addr, uint32_t word_cnt);
    //read a byte from specific port
    uint8_t io_inb(uint16_t port);
    //read a unsigned interger from specific port
    uint32_t io_inl(uint16_t port);
    //read bytes from specific port to specific address
    void io_insw(uint16_t port, void* addr, uint32_t word_cnt);

}
#endif