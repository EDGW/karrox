
#ifndef _KERNEL_PRINT_H
/*  Basic Print Functions In Kernel     */
#define _KERNEL_PRINT_H
#include <stdint.h>
namespace boot{
    /*  8 bit printing color            */
    typedef uint8_t color8b_t;
    #define ON          true
    #define OFF         false
    #define CLR_RED     (color8b_t)0b100
    #define CLR_GREEN   (color8b_t)0b010
    #define CLR_BLUE    (color8b_t)0b001
    #define CLR_WHITE   (CLR_RED | CLR_GREEN |CLR_BLUE)

    void set_printer_color(color8b_t clr);      //set the printer's foreground color
    void set_printer_background(color8b_t clr); //set the printer's background color
    void set_printer_highlight(bool);           //ON/OFF determines whether the characters is highlighted or not
    void set_printer_flashing(bool);            //ON/OFF determines whether the characters is flashing or not


    /*  initialize screen printer       */
    void init_printer();
    /*  print a character on screen     */
    void putchar(char c);
    /*  print a string on screen        */
    void putstr(const char* str);
    /*  putstr tailored for asm        */
    extern "C" void asm_putstr(char* str);
    /*  number characters in radix    */
    extern char radixnum[];
    /*  print an interger with specific prefix and radix    */
    template<typename T>
    void putint(T value,const char* prefix,int32_t radix){
        if(value<(T)0){
            putchar('-');
        }
        putstr(prefix);
        if(value==(T)0){
            putchar('0');
            return;
        }
        //preload part ends here
        T base = 1;
        while(value/base >=radix)base*=radix;
        while(base>0){
            putchar(radixnum[value/base]);
            value%=base;
            base/=radix;
        }
    }
    /*  print a decimal number*/
    template<typename T>
    void putint(const T value){
        putint(value,"",10);
    }
    template<typename T>
    void putintb(const T value){
        putint(value,"0b",2);
    }
    template<typename T>
    void putinto(const T value){
        putint(value,"0",8);
    }
    template<typename T>
    void putinth(const T value){
        putint(value,"0x",16);
    }

    /*  roll up a line                  */
    void rollscreen();
    /*  formatted print a string and a few arguments  
        This is a simple implementation and only supports the following replacers:
        %s: indicates the character will be replaced by a string
        %d: indicates the character will be replaced by a decimal interger
        %o: indicates the character will be replaced by a octal interger
        %x %X: indicates the character will be replaced by a hexadecimal interger
        %b: the additional replacer indicating the charater will be replaced by a binary interger which is not in any standards
        **DIFFERS FROM C/C++ STANDARD**
        every printed undecimal interger will be with a prefix such as '0x114514'
        */
    extern "C" void printf(const char* format...);


    
}
#endif