#include "boot.h"
#include "print.h"
#include "arch.h"
#include <stdint.h>
#include "memory.h"
namespace arch_x86{
    /*  create a global descriptor table entry
    base:           32bit-addr
    limit:          20bit-limit in granularity
    flags/access:   gdt properties
    */
    uint64_t create_gdt_entry(uint64_t base,uint64_t limit,uint64_t flags,uint64_t access){
        uint64_t entry = flags | access;
        entry |= ((base&0xffffu)<<16);/*16-31 stores the 0-15 bits of the base */
        entry |= ((base&0xff0000u)<<(32-16));/*32-39 stores the 16-23 bits of the base*/
        entry |= ((base&0xff000000u)<<(56-24));/*56-63 stores the 24-31 bits of the base*/

        entry |= ((limit&0xffffu));
        entry |= ((limit&0xf0000u)<<(48-16));
        return entry;
    }
    void init_gdt(){
        boot::putstr("initializing global descriptor table.\n");
        uint64_t gdtr = 0;
        asm volatile("sgdt %0"::"m"(gdtr):);
        uint32_t offset = (uint32_t)(gdtr>>16); //GDTR REG: 48-16 offset;  15-0 limit
        uint64_t* table = (uint64_t*)offset;
        *(table+0)=create_gdt_entry(0,0xfffffu,DEFAULT_FLAG,ACCESS_CODE | ACCESS_USER);//0x0 user code
        *(table+1)=create_gdt_entry(0,0xfffffu,DEFAULT_FLAG,ACCESS_DATA | ACCESS_USER);//0x8 user data
        *(table+2)=create_gdt_entry(0,0xfffffu,DEFAULT_FLAG,ACCESS_CODE | ACCESS_KERNEL);//0x10 kernel code
        *(table+3)=create_gdt_entry(0,0xfffffu,DEFAULT_FLAG,ACCESS_DATA | ACCESS_KERNEL);//0x18 kernel data
        asm volatile("lgdt %0"::"m"(gdtr):);
    }
    void init_paging(){

    }
    void init_memory(){
        init_gdt();
        init_paging();
    }
    void main(){
        boot::main();
    }
}
const char* arch::getbootinfo(){
        return "GRUB x86 kernel boot";
}
void arch::init(){
    arch_x86::init_memory();
}
extern "C" void boot_x86_main(void){
    arch_x86::main();
}