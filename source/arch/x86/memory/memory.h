#ifndef _X86_MEMORY_H
#define _X86_MEMORY_H
#include "stdint.h"
#define KERNEL_MEM_START_ADDR   0xc0000000  //[3,4)GiB memory

#pragma region GLOBAL_DESCRIPTOR_TABLE

/**
 * @brief access flags of gdt.
 * @details access flag structure:
 *          Present | DescPrivilegeLevel | S(Type) | Executable | Direction/Conforming | RW | Accessed
*/
#define GDT_ACCESS              0
#define GDT_ACCESS_P            0b10000000  // Present,Allows an entry to refer to a valid segment. Must be set (1) for any valid segment. 
#define GDT_ACCESS_DPL0         0b00000000  /* Descriptor privilege level field. Contains the CPU Privilege level of the segment. 
                                            0 = highest privilege (kernel). */
#define GDT_ACCESS_DPL3         0b01100000  /* Descriptor privilege level field. Contains the CPU Privilege level of the segment. 
                                            3 = lowest privilege (user applications). */
#define GDT_ACCESS_S_SYS        0b00000000  // Descriptor type bit, determines it's a system desciptor.
#define GDT_ACCESS_S_TEXT       0b00010000  // Descriptor type bit, determines it's a text desciptor (code/data).
#define GDT_ACCESS_EXECUTABLE   0b00001000  // Executable bit, determines it's executable segment.
#define GDT_ACCESS_DIR_UP       0b00000000  // Direction bit, determines this data segment grows up.
#define GDT_ACCESS_DIR_DOWN     0b00000100  // Direction bit, determines this data segment grows down.
#define GDT_ACCESS_CONFORMING   0b00000100  // Comforming bit, the segment can be executed from a lower privilege level if set.
#define GDT_ACCESS_CODE_RW_E    0b00000000  //Code RW bit,determines the segment is executable
#define GDT_ACCESS_CODE_RW_RE   0b00000010  //Code RW bit,determines the segment is readable and executable
#define GDT_ACCESS_DATA_RW_R    0b00000000  //Code RW bit,determines the segment is readable
#define GDT_ACCESS_DATA_RW_RW   0b00000010  //Code RW bit,determines the segment is readable and writable

#define GDT_ACCESS_KERNEL       (GDT_ACCESS_P | GDT_ACCESS_DPL0 | GDT_ACCESS_S_TEXT)
#define GDT_ACCESS_USER         (GDT_ACCESS_P | GDT_ACCESS_DPL3 | GDT_ACCESS_S_TEXT)
#define GDT_ACCESS_CODE         (GDT_ACCESS_EXECUTABLE | GDT_ACCESS_CODE_RW_RE )
#define GDT_ACCESS_DATA         (GDT_ACCESS_DIR_UP | GDT_ACCESS_DATA_RW_RW )

#define GDT_FLAG_G_4K   0b1000         // determines the granularity flag, which indicates the size the Limit value is scaled by, is 4KiB.
#define GDT_FLAG_G_1B   0b0000         // determines the granularity flag, which indicates the size the Limit value is scaled by, is 1Byte.
#define GDT_FLAG_DBL_16 0b0000
#define GDT_FLAG_DBL_32 0b0100
#define GDT_FLAG_DBL_64 0b0010

#define GDT_FLAG_32     (GDT_FLAG_G_4K | GDT_FLAG_DBL_32)

#define GDT_SELECTOR_RPL0 0b00
#define GDT_SELECTOR_RPL3 0b11
#define GDT_SELECTOR_USER_CODE (0x0 | GDT_SELECTOR_RPL3)
#define GDT_SELECTOR_USER_DATA (0x8 | GDT_SELECTOR_RPL3)
#define GDT_SELECTOR_KERNEL_CODE (0x10 | GDT_SELECTOR_RPL0)
#define GDT_SELECTOR_KERNEL_DATA (0x18 | GDT_SELECTOR_RPL0)
#pragma endregion //GLOBAL_DESCRIPTOR_TABLE

#ifndef ASM_FILE
/**
 * @brief the pointer type to the global descriptor table
*/
typedef int64_t* gdt_ptr;


/**
 * @brief initialize memory layout
*/
void init_memory();
#endif //nASM_FILE

#endif //_X86_MEMORY_H