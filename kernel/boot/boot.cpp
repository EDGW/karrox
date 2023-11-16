#include "boot.h"
#include "print.h"
#include "arch/arch.h"
#include "arch/io.h"
#include "lib/debug.h"
#include "drive/pci/pci.h"
namespace boot{


    #ifdef DEBUG
    void debug(){
        set_printer_background(CLR_RED);
        printf("----  debug printer ----\nmake sure this line won't be shown in any releases\n");
        reset_printer_color();
        drive::pci::debug();
    }
    #endif

    void printinfo(){
        printf("booting from %s.\n",arch::getbootinfo());
        printf("karrox kernel started from %x.\n",printinfo);
    }

    /*  kernel starts from here     */
    void main(){
        init_printer();
        printinfo();
        arch::init();
        drive::pci::init();
        printf("initialization finished.\n");
        #ifdef DEBUG
        debug();
        #endif
        while(1);
    }
}