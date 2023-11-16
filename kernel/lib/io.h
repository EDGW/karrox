#ifndef _LIB_IO_H
#define _LIB_IO_H

#include "boot/arch/io.h"
//write a byte to specific port
inline void outb(uint16_t port, uint8_t data){
    io::io_outb(port,data);
}
//write a unsigned interger to specific port
inline void outl(uint16_t port, uint32_t data){
    io::io_outl(port,data);
}
//write 'word_cnt' bytes from addr to specific port
inline void outsw(uint16_t port, const void* addr, uint32_t word_cnt){
    io::io_outsw(port,addr,word_cnt);
}
//read a byte from specific port
inline uint8_t inb(uint16_t port){
    return io::io_inb(port);
}
//read a unsigned interger from specific port
inline uint32_t inl(uint16_t port){
    return io::io_inl(port);
}
//read bytes from specific port to specific address
inline void insw(uint16_t port, void* addr, uint32_t word_cnt){
    return io::io_insw(port,addr,word_cnt);
}
#endif