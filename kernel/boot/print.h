
#ifndef _KERNEL_PRINT_H
/*  Basic Print Functions In Kernel     */
#define _KERNEL_PRINT_H
#include <stdint.h>
namespace boot{
    /*  8 bit printing color            */
    typedef uint8_t color8b_t;

    /*  initialize screen printer       */
    void init_printer();
    /*  print a character on screen     */
    void putchar(char c);
    /*  print a string on screen        */
    void putstr(const char* str);
    /*  roll up a line                  */
    void rollscreen();
    /*  formatted print a string and a few arguments  */
    extern "C" void printf(const char* format...);
}
#endif