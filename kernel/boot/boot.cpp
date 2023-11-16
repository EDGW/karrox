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
        while(1);
    }
}