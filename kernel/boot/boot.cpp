#include "boot.h"
#include "print.h"
#include "arch/arch.h"
namespace boot{
    void printinfo(){
        printf("booting from %s.\n",arch::getbootinfo());
        printf("karrox kernel started from %x.\n",printinfo);
    }
    /*  kernel starts from here     */
    void main(){
        init_printer();
        printinfo();
        arch::init();
        printf("initialization finished.\n");
        uint32_t u;
        asm("movl $0x1919810,%eax");
        while(1){
            while(1){
                u++;
            }
        }
    }
}