/**
 * @file page.c
 * @brief basic init and management functions of paging
 * @details The main memory was remapped into different pages.
 *          Every process has it's virtual 4GB memory in the flat space, 
 *          the memory management system is aim to assign every process's memory pages from
 *          the limited physical memory pages, store the fewer-time used page to the disk and
 *          arrange new pages when a process needs a page.
*/
#include "memory.h"
#include "paging.h"
#include "../bootinfo.h"


#pragma region PAGE_TABLE_INITIALIZER

/**
 * @brief the size of physical memory pages.
*/
uint32_t physical_page_count = 0x0;

/**
 * @brief clear 4KiB memory from the specific addr
*/
static inline void clear_mem_4k(void* addr){
    uint64_t *u = (uint64_t*)addr;
    for(int i = 0;i<64;i++){
        *(u+i)=0;
    }
}

/**
 * @brief create a primary or secondary page table entry
 * @param addr the page address this page entry points to
 * @param flags 8-bit(primary entry) or 9-bit(secondary entry) flags of this entry
*/
static inline uint32_t create_page_entry(uint32_t addr,uint16_t flags){
    return (addr & 0xfffff000) | flags;
}

/**
 * @brief add a secondary page table to the primary page table
 * @param index the index of this entry in the primary page table
 * @param page_table_addr the address if the secondary page table
 * @param flags 8-bit flags of this primary page table entry
*/
static inline void create_page_directory_entry(uint16_t index,void *page_table_addr,uint8_t flags){
    uint32_t *pt = (uint32_t*)KERNEL_PT_POS;
    *(pt+index)=create_page_entry((uint32_t)page_table_addr,flags);
}

/**
 * @brief create a secondary page table which covers a section of 4-MiB memory
 * @param index the index of this secondary table in page table
 * @param start_addr the physicall address where this entry covers from
 * @param flags 9-bit flags of this primary secondary table entry
*/
static inline void create_secondary_page_table(uint16_t index,void* start_addr,uint16_t flags){
    uint32_t *ptable = (uint32_t*)(KERNEL_PT_POS+(0x1000*(index+1)));
    for(int i = 0;i<1024;i++){
        *ptable=create_page_entry((uint32_t)start_addr,flags);
        ptable++,start_addr+=0x1000;
    }
}

/**
 * @brief reload page table with the specific page table
 * @param page_table the address of page table
*/
void reload_page(void* page_table){
    asm volatile(
        "movl %0,%%eax\n"
        "movl %%eax,%%cr3"
        :
        :"r"((uint32_t)page_table)
        :"%eax"
    );
}

/**
 * @brief create the page table
*/
void create_page_table(){
    clear_mem_4k((void*)PHYSICAL_KERNEL_PT_POS);
    //create lower_32MiB table
    void *ptaddr = (void*)(PHYSICAL_KERNEL_PT_POS+0x1000);//skip page directory table
    for(int i = 0;i<8;i++){
        create_page_directory_entry(i,ptaddr+(i*0x1000),PG_FLAG_P | PG_FLAG_RW_RW | PG_FLAG_US_S);
        create_page_directory_entry(i+768,ptaddr+(i*0x1000),PG_FLAG_P | PG_FLAG_RW_RW | PG_FLAG_US_S);
        create_secondary_page_table(i,(void*)(i*0x400000),PG_FLAG_GLOBAL | PG_FLAG_P |PG_FLAG_RW_RW | PG_FLAG_US_S);
    }
    reload_page((void*)PHYSICAL_KERNEL_PT_POS);
}

/**
 * @brief init paging
 * @details Physically, lower [0,2) MiB is used by BIOS, 
 *          the operating system should avoid to use the lower 2-MB memory or arrange it to other processes,
 *          [2,32) MiB memory is used by the opreating system kernel,
 *          specially ,[4,12) MiB memory is kernel Page Table,
 *          other momory pages are user-available
 *          Virtually, kernel used higher [3,4) GiB memory and other memory are user-available.
 *          
*/
void init_paging(){//extern "paging.h"
    uint32_t uppermemsiz = get_bootinfo()->memory.upper_mem_size;
    physical_page_count = (uppermemsiz + 1024)>>2; //(upper memory + lower memory(1MiB)) / the size of a single page in kilobytes(4)
    create_page_table();
}

#pragma endregion //PAGE_TABLE_INITIALIZER

