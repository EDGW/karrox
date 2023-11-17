/**
 * @file io.h
 * @brief a header file describes the basic operation in architectures
 * the more common interface is @ref ./kernel/lib/io.h
*/
#ifndef _ARCH_IO_H
#define _ARCH_IO_H /*      A inline code set to provide some basic io(not disk io) operations       */
#include <stdint.h>

/**
 * @brief a namespace contains the basic operation in architectures, 
 * it's also a common interface that any boots should implements
*/
namespace io{
    /**
     * @brief write a byte to the specific port
     * @param port the port number (between 0 - 65535) to write
     * @param data the 8 bit data
    */
    void io_outb(uint16_t port, uint8_t data);
    /**
     * @brief write a long to the specific port
     * @param port the port number (between 0 - 65535) to write
     * @param data the 32 bit data
    */
    void io_outl(uint16_t port, uint32_t data);
    /**
     * @brief out a sequence of bytes to the specific port
     * @param port the port number (between 0 - 65535) to write
     * @param addr the pointer stores the data
     * @param word_cnt the size of data
    */
    void io_outsw(uint16_t port, const void* addr, uint32_t word_cnt);
    /**
     * @brief read a byte from the specific port
     * @param port the port number (between 0 - 65535) to read
     * @return a 8-bit value read from the spec port
    */
    uint8_t io_inb(uint16_t port);
    /**
     * @brief read a long from the specific port
     * @param port the port number (between 0 - 65535) to read
     * @return a 32-bit value read from the spec port
    */
    uint32_t io_inl(uint16_t port);
    /**
     * @brief read a sequence of bytes from the specific port
     * @param port the port number (between 0 - 65535) to read
     * @param[out] addr where the data read from the spec port stores
     * @param word_cnt the size of data
    */
    void io_insw(uint16_t port, void* addr, uint32_t word_cnt);

}
#endif