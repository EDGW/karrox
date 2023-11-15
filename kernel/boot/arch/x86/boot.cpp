#include "boot.h"
#include "arch.h"
namespace arch_x86{
    void main(){
        boot::main();
    }
}
const char* arch::getbootinfo(){
        return "GRUB x86 kernel boot";
}
extern "C" void boot_x86_main(void){
    arch_x86::main();
}