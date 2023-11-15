#include "boot.h"
#include "print.h"
#include "arch.h"
namespace arch_x86{
    void main(){
        boot::main();
    }
}
const char* arch::getbootinfo(){
        return "GRUB x86 kernel boot";
}
void arch::init(){
    boot::putstr("initialize interrupt table\n");
}
extern "C" void boot_x86_main(void){
    arch_x86::main();
}