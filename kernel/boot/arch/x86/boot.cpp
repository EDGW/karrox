#include "boot.h"
#include "print.h"
#include "arch.h"
#include <stdint.h>
#include "memory.h"
#include "interrupt.h"
#include "io.h"
    extern "C" void hand_intr_handler3();
namespace arch_x86
{
    /*      GDT         */
    /*  create a global descriptor table entry
    base:           32bit-addr
    limit:          20bit-limit in granularity
    flags/access:   gdt properties
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


    /*      Paging      */
    //clear 4Kib memory from ptr
    void clear4k(uint8_t* ptr){
        for(int i = 0;i<0x1000;i++){
            *ptr=0;
            ptr++;
        }
    }
    /*1024:page directory table siz; 4096:4 page tables siz
    PDE#0->PT#1
    PDE#1->PT#2
    PDE#2->PT#3
    PDE#3->PT#4
    PDE#4->PT#5
    PDE#0x300->PT#1
    PDE#0x301->PT#2
    PDE#0x302->PT#3
    PDE#0x303->PT#4
    PDE#0x304->PT#5
    PE#1 (0-4MiB)
    PE#2 (5-8MiB)
    PE#3 (9-12MiB)
    PE#4 (13-16MiB)
    PE#5 (17-20MiB)
    */
    pg_table_entry* kernel_page=(pg_table_entry*)0x1000000;//16MB+1
    /*
    create a page entry
    offset: 32bit physical page addr,make sure the low 12 bit is 0
    flags:  page flags
    */
    pg_table_entry create_pg_entry(uint32_t offset,uint32_t flags){
        offset &= 0xfffff000;
        offset |= flags;
        return offset;
    }
    void create_pde(){
        clear4k((uint8_t*)kernel_page);
        for(int i = 0;i<5;i++){
            kernel_page[i]=kernel_page[0x300+i]
            =create_pg_entry((uint32_t)(kernel_page+(0x400*(i+1))),PAGE_FLAG_DEFAULT_USER);
        }
    }
    void create_pte(uint32_t id,uint32_t phyAddr,uint32_t flags){
        for(uint32_t i = 0;i<1024;i++){
            kernel_page[(id)*0x400+i]=create_pg_entry(phyAddr+(i*4096),flags);
        }
    }
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
    
    
    /*      IDT     
        0x00-0x1F Exceptions
        0x20-0x2f User-defined Interrupts
    */
    /*create a intr entry
    offset  :32-bit handler address
    selector:handler's segment selector
    flag    :intr flags
    */
    intr_desc_entry create_intr_entry(uint32_t offset,uint16_t selector,uint8_t flag){
        intr_desc_entry desc;
        desc.offset_low=(uint16_t)(offset&0xffff);
        desc.offset_high=(uint16_t)((offset&0xffff0000)>>16);
        desc.flag=flag;
        desc.seg_selector=selector;
        return desc;
    }
    intr_desc_entry table[INTR_CNT];
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
    /*      init programmable interrupt controller  (8259A)*/
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
    void init_intr(){
        create_intr_table();
        init_pic();
        asm volatile("sti");
    }
    void init_memory()
    {
        init_gdt();
        init_paging();
        init_intr();
    }
    void main()
    {
        boot::main();
    }
}
const char *arch::getbootinfo()
{
    return "GRUB x86 kernel boot";
}
void arch::init()
{
    arch_x86::init_memory();
}
extern "C" void boot_x86_main(void)
{
    arch_x86::main();
}