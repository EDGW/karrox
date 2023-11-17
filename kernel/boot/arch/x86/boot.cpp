#include <stdint.h>
#include "boot.h"
#include "print.h"
#include "arch.h"
#include "memory.h"
#include "interrupt.h"
#include "io.h"
#include "multiboot2.h"
#include "systeminfo.h"

using namespace io;
namespace arch_x86
{
    /**
     * @brief initialization of global descriptor table
    */
    #pragma region GDT_Init
    
    /**
     * @brief create a global descriptor table entry
     * @param base  32bit-addr this segment starts from
     * @param limit 20bit-limit in granularity determines the size of the segment
     * @param flags  segment flags
     * @param access segment accessible flags
     */
    uint64_t create_gdt_entry(uint64_t base, uint64_t limit, uint64_t flags, uint64_t access)
    {
        uint64_t entry = flags | access;
        entry |= ((base & 0xffffu) << 16);            /*16-31 stores the 0-15 bits of the base */
        entry |= ((base & 0xff0000u) << (32 - 16));   /*32-39 stores the 16-23 bits of the base*/
        entry |= ((base & 0xff000000u) << (56 - 24)); /*56-63 stores the 24-31 bits of the base*/

        entry |= ((limit & 0xffffu));
        entry |= ((limit & 0xf0000u) << (48 - 16));
        return entry;
    }
    
    /**
     * @brief initialize global descriptor table
    */
    void init_gdt()
    {
        boot::putstr("initializing global descriptor table.\n");
        uint64_t gdtr = 0;
        asm volatile("sgdt %0" ::"m"(gdtr) :);
        uint32_t offset = (uint32_t)(gdtr >> 16); // GDTR REG: 48-16 offset;  15-0 limit
        uint64_t *table = (uint64_t *)offset;
        *(table + 0) = create_gdt_entry(0, 0xfffffu, DEFAULT_FLAG, ACCESS_CODE | ACCESS_USER);   // 0x0 user code
        *(table + 1) = create_gdt_entry(0, 0xfffffu, DEFAULT_FLAG, ACCESS_DATA | ACCESS_USER);   // 0x8 user data
        *(table + 2) = create_gdt_entry(0, 0xfffffu, DEFAULT_FLAG, ACCESS_CODE | ACCESS_KERNEL); // 0x10 kernel code
        *(table + 3) = create_gdt_entry(0, 0xfffffu, DEFAULT_FLAG, ACCESS_DATA | ACCESS_KERNEL); // 0x18 kernel data
        asm volatile("lgdt %0" ::"m"(gdtr) :);
    }
    
    #pragma endregion //GDT_Init

    /**
     * @brief initialization of page table
     * @details
     * 1024:page directory table siz; 4096:4 page tables siz
     * PDE#0->PT#1
     * PDE#1->PT#2
     * PDE#2->PT#3
     * PDE#3->PT#4
     * PDE#4->PT#5
     * PDE#0x300->PT#1
     * PDE#0x301->PT#2
     * PDE#0x302->PT#3
     * PDE#0x303->PT#4
     * PDE#0x304->PT#5
     * PE#1 (0-4MiB)
     * PE#2 (5-8MiB)
     * PE#3 (9-12MiB)
     * PE#4 (13-16MiB)
     * PE#5 (17-20MiB)
    */
    #pragma region PAGING_Init
    
    /**
     * @brief clear 4Kib memory from specific address
     * @param ptr a pointer to the address
    */
    void clear4k(uint8_t* ptr){
        for(int i = 0;i<0x1000;i++){
            *ptr=0;
            ptr++;
        }
    }
    
    /** a pointer determines where the kernel page table stores*/
    pg_table_entry* kernel_page=(pg_table_entry*)0x1000000;//16MB+1
    
    /**
     * @brief create a page entry
     * @param offset a 32-bit physical page addr,make sure it's aligned in 4 KiB
     * @param flags  page flags
    */
    pg_table_entry create_pg_entry(uint32_t offset,uint32_t flags){
        offset &= 0xfffff000;
        offset |= flags;
        return offset;
    }
    
    /**
     * @brief create the page directory table or the primary page table
    */
    void create_pde(){
        clear4k((uint8_t*)kernel_page);
        for(int i = 0;i<5;i++){
            kernel_page[i]=kernel_page[0x300+i]
            =create_pg_entry((uint32_t)(kernel_page+(0x400*(i+1))),PAGE_FLAG_DEFAULT_USER);
        }
    }

    /**
     * @brief create the page table or the secondary page table
     * @details
     * Page Table Layout:                               
     * #0 Page Directory Table  (Primary Page Table)
     * #1 Page Table 1          (Secondary Page Table)
     * #2 Page Table 2          (Secondary Page Table)
     * #...
     * @param id the index of this page table in seconday page tables starts from 1 or in all tables starts from 0.
    */
    void create_pte(uint32_t id,uint32_t phyAddr,uint32_t flags){
        for(uint32_t i = 0;i<1024;i++){
            kernel_page[(id)*0x400+i]=create_pg_entry(phyAddr+(i*4096),flags);
        }
    }
    
    /**
     * @brief initialize paging
    */
    void init_paging()
    {
        boot::putstr("initializing page table.\n");
        create_pde();
        uint32_t pg_addr = 0;
        for(int i = 1;i<=5;i++,pg_addr+=4096){
            create_pte(i,pg_addr,PAGE_FLAG_DEFAULT_KERNEL);
        }
        asm volatile(
            "mov %0     ,%%eax\n"
            "mov %%eax  ,%%cr3\n"
            "mov %%cr0  ,%%eax\n"
            "or $0x80000001,%%eax\n"
            "mov %%eax  ,%%cr0\n"
            :
            :"r"(kernel_page)
            :"%eax"
        );
    }
    #pragma endregion //PAGING_Init
    
    /**
     * @brief initialization of interrupt table and hardwares
     * @details   
     * INT numbers:
     * 0x00-0x1F Exceptions
     * 0x20-0x2f User-defined Interrupts
    */
    #pragma region INTR_Init

    /**
     * @brief create an intr entry
     * @param offset 32-bit handler address
     * @param selector handler's segment selector
     * @param flag intr flags
    */
    intr_desc_entry create_intr_entry(uint32_t offset,uint16_t selector,uint8_t flag){
        intr_desc_entry desc;
        desc.offset_low=(uint16_t)(offset&0xffff);
        desc.offset_high=(uint16_t)((offset&0xffff0000)>>16);
        desc.flag=flag;
        desc.seg_selector=selector;
        return desc;
    }
    
    /** interrupt descriptor table  */
    intr_desc_entry table[INTR_CNT];    //interrupt descriptor table
    
    /**
     * @brief create interrupt descriptor table
     * */
    void create_intr_table(){
        boot::putstr("initializing interrupt table.\n");
        uint32_t* u = &intr_handler_table;
        for(int i = 0;i<INTR_CNT;i++){
            uint32_t addr = *u;
            table[i]=create_intr_entry(addr,SELECTOR_KERNEL_CODE,INTR_FLAG_DEFAULT_DPL0);
            u++;
        }
        uint64_t idtr = ((uint64_t)(uint32_t)table)<<16;
        idtr |= sizeof(table)-1;
        asm volatile("lidt %0"::"m"(idtr):);
    }
    
    /**
     * @brief init programmable interrupt controller  (8259A)
     */
    void init_pic(){
        boot::printf("initializing programmable interrupt controller(8259A)\n");
        //Init MASTER
        io_outb(X8259A_MASTER_1_PORT,ICW1_IC4 | ICW1 | ICW1_LTIM_EDGE); //ICW1
        io_outb(X8259A_MASTER_2_PORT,USER_INTR_ST);         //ICW2 determines that the intr vector starts from 0x20 IR[0-7]->0x20-0x27
        io_outb(X8259A_MASTER_2_PORT,0b00000100);   //ICW3 determines that the 2nd port connects the slave
        io_outb(X8259A_MASTER_2_PORT,0b00000001);   //8086 mode, normal EOI (the enf of an intr is handled by handler but hardware)
        //Init SLAVE
        io_outb(X8259A_SLAVE_1_PORT,ICW1_IC4 | ICW1 | ICW1_LTIM_EDGE); //ICW1
        io_outb(X8259A_SLAVE_2_PORT,USER_INTR_ST+8);         //ICW2 determines that the intr vector starts from 0x28 [IR8-15]->0x28-0x2f
        io_outb(X8259A_SLAVE_2_PORT,0b00000010);   //ICW3 determines that the slave connects to the 2nd port of the master
        io_outb(X8259A_SLAVE_2_PORT,0b00000001);   //8086 mode, normal EOI (the enf of an intr is handled by handler but hardware)
        
        //print the intr table
        boot::set_printer_color(CLR_GREEN);
        uint32_t num = USER_INTR_ST;
        for(int i = 0;i<=15;i+=1,num++){
            boot::printf("IRQ%d:%x\t",i,num);
            if((i+1)%4==0)boot::putchar('\n');
        }
        boot::reset_printer_color();
    }
    
    /** 
     * @brief init interrupt      
     */
    void init_intr(){
        create_intr_table();
        init_pic();
        init_handlers();
        asm volatile("sti");
    }
    
    #pragma endregion


    /** multiboot info pointer */
    multiboot_info* mboot_info;

    /** system info */

    arch::system_info sys_info;
    /**
     * @brief init of memory layouts
    */
    void init_memory()
    {
        init_gdt();
        init_paging();
    }

    /**
     * @brief parse a multiboot tag and update the system info
    */
    void parse_tag(multiboot_tag* tag){
        switch (tag->type)
        {
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
        {
            auto* info = (multiboot_tag_basic_meminfo*)tag;
            sys_info.mem_info.mem_lower = info->mem_lower;
            sys_info.mem_info.mem_upper = info->mem_upper;
            break;
        }
        case MULTIBOOT_TAG_TYPE_BOOTDEV:
        {
            auto* info = (multiboot_tag_bootdev*)tag;
            sys_info.boot_device.device_id=info->biosdev;
            sys_info.boot_device.partition=info->slice;
            sys_info.boot_device.sub_partition=info->part;
            break;
        }
        case MULTIBOOT_TAG_TYPE_APM:
        {
            auto* info = (multiboot_tag_apm*)tag;
            sys_info.apm.verison = info->version;
            sys_info.apm.cseg = info->cseg;
            sys_info.apm.offset = info->offset;
            sys_info.apm.cseg_16 = info->cseg_16;
            sys_info.apm.dseg = info->dseg;
            sys_info.apm.flags = info->flags;
            sys_info.apm.cseg_len = info->cseg_len;
            sys_info.apm.cseg_16_len = info->cseg_16_len;
            sys_info.apm.dseg_len = info->dseg_len;
            break;
        }
        default:
            //don't parse it, do nothing;
            break;
        }
    }

    /**
     * @brief initialize boot information
    */
    void init_bootinfo(){
        boot::printf("load info from multiboot tags(%d)\n",mboot_info->total_size);
        multiboot_tag* tag_ptr = mboot_info->tags;
        uint32_t tag_sz = mboot_info->total_size - (sizeof(multiboot_uint32_t))*2;
        for(uint32_t offset = 0;offset < tag_sz;){
            parse_tag(tag_ptr);
            uint32_t nx_addr = ((uint32_t)tag_ptr)+tag_ptr->size;
            if(nx_addr%8!=0){       //8 bytes alignment
                nx_addr=nx_addr - (nx_addr%8) +8;
            }
            offset+=nx_addr - ((uint32_t)(tag_ptr));
            tag_ptr = (multiboot_tag*)nx_addr;
        }
    }


    /*  main function   */
    void main(multiboot_info *info)
    {
        mboot_info = info;
        boot::main();
    }

}

/**
 * @brief implementation of 'arch.h'
*/
#pragma region ARCH_IMPL

    const char *arch::getbootinfo()
    {
        return "GRUB x86 kernel boot";
    }
    void arch::init()
    {
        arch_x86::init_bootinfo();
        arch_x86::init_memory();
        arch_x86::init_intr();
    }
    arch::system_info* arch::getsysteminfo(){
        return &arch_x86::sys_info;
    }
#pragma endregion //ARCH_IMPL

/**
 * @brief the starting point of boot after stack was initialized.
*/
#pragma region BOOT_MAIN
/**
 * @brief the starting point of boot after stack was initialized.
 * @param info a pointer to the multiboot_info struct
*/
extern "C" void boot_x86_main(multiboot_info *info)
{
    arch_x86::main(info);
}
#pragma endregion //BOOT_MAIN