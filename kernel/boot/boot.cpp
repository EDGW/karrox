#include "boot.h"
#include "print.h"
#include "arch/arch.h"
namespace boot{
    void printinfo(){
        printf("booting from %s\n",arch::getbootinfo());
        printf("karrox kernel started");
    }
    /*  kernel starts from here     */
    void main(){
        init_printer();
        printinfo();
        while(1){
        }
    }
}