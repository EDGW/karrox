/**
 * @file io.h
 * @brief This file provides a set of inline functions of kernel io operations.
*/
#ifndef _LIB_KERNEL_IO_H
#define _LIB_KERNEL_IO_H
#include "stdint.h"

/**
 * @brief write a byte to the specific port
 * @param port the specific port
 * @param value the data to write
*/
static inline void kernel_io_outb(uint16_t port,uint8_t value){
   asm volatile ( "outb %0, %w1" : : "a" (value), "Nd" (port));    
}

/**
 * @brief read a byte from the specific port
 * @param port the specific port
*/
static inline uint8_t inb(uint16_t port) {
   uint8_t data;
   asm volatile ("inb %w1, %b0" : "=a" (data) : "Nd" (port));
   return data;
}

#endif //_LIB_KERNEL_IO_H