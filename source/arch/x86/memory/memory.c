/**
 * @file memory.c
 * @brief x86 memory management
*/
#
#include "boot/memory.h"
#include "stdint.h"
#include "memory.h"
#include "paging.h"

/**
 * @brief basic init and management functions of GDT
*/
#pragma region GLOBAL_DESCRIPTOR_TABLE

/**
 * @brief get gdtr printer
 * @param offset where the gdt offset is stored
 * @param limit where the gdt limit is stored
*/
uint64_t get_gdtr(gdt_ptr *offset,uint16_t *limit){
    uint64_t gdtr = 0;
    asm volatile("sgdt %0"::"m"(gdtr));
    *limit = (uint16_t)(gdtr & 0xffff);
    *offset = (gdt_ptr)(uint32_t)(gdtr >> 16);
    return gdtr;
}

/**
 * @brief load gdtr pointer
 * @param offset the offset of global descriptor table
 * @param limit the length of global descriptor table in bytes
*/
void load_gdtr(gdt_ptr offset,uint16_t limit){
    /*
    GDTR structure:
    [48  offset  16][15  limit  0]
    */
    uint64_t gdtr = ((uint64_t)(uint32_t)offset)<<16;
    gdtr |= limit;
    asm volatile("lgdt %0"::"m"(gdtr));
}

/**
 * @brief set gdt entry at the specific index
 * @param table the pointer to gdt
 * @param index the index of this entry in gdt
 * @param base_ptr  the beginning of this segment
 * @param limit     the limit of this segment in granularity
 * @param flags     the flags of the segment (granularity / size / long mode)
 * @param access    the access flag of the segment , see macros @ref GDT_ACCESS_...
*/
void set_gdt_entry(gdt_ptr table,uint16_t index,uint32_t *base_ptr,uint32_t limit,uint8_t flags,uint8_t access){
    uint32_t base = (uint32_t)base_ptr;
    uint64_t entry = 0;
    entry |= ((uint64_t)limit & 0xffff)<<(0-0);           //[15-0] limit[0-15]
    entry |= ((uint64_t)base & 0xffff)<<(16-0);           //[31-16] base[0-15]
    entry |= ((uint64_t)base & 0xff0000)<<(32-16);        //[39-32] base[16-13]
    entry |= (uint64_t)access<<40;                        //[47-40] access
    entry |= ((uint64_t)limit & 0xff0000)<<(48-16);       //[51-48] limit[16-19]
    entry |= ((uint64_t)flags & 0b1111)<<(52-0);          //[55-52] flags
    entry |= ((uint64_t)base & 0xff000000)<<(56-24);      //[56-63] base[24-31]
    *(table+index) = entry;
}

/**
 * @brief initialize global descriptor table
 * GDT #0 (0x0) User Code SEG
 * GDT #1 (0x8) User Data SEG
 * GDT #2 (0x10) Kernel Code SEG
 * GDT #3 (0x18) Kernel Data SEG
*/
void init_gdt(){
    gdt_ptr offset;uint16_t lm;
    get_gdtr(&offset,&lm);
    set_gdt_entry(offset,0,0,0xffffff,GDT_FLAG_32,GDT_ACCESS_USER | GDT_ACCESS_CODE);
    set_gdt_entry(offset,1,0,0xffffff,GDT_FLAG_32,GDT_ACCESS_USER | GDT_ACCESS_DATA);
    set_gdt_entry(offset,2,0,0xffffff,GDT_FLAG_32,GDT_ACCESS_KERNEL | GDT_ACCESS_CODE);
    set_gdt_entry(offset,3,0,0xffffff,GDT_FLAG_32,GDT_ACCESS_KERNEL | GDT_ACCESS_DATA);
    lm = 4*8 - 1;
    load_gdtr(offset,lm);
}

#pragma endregion //GLOBAL_DESCRIPTOR_TABLE



void init_memory(){ //extern "memory.h"
    init_gdt();
    init_paging();
}
