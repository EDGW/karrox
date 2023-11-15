#include "print.h"
#include <stdint.h>
#include <stdarg.h>
/*  the implementation of printing functions*/
namespace boot{
    #define STRING_END '\0' //the end flag of a string
    char* charpointer;      //a pointer to graphics ram
    uint32_t screenwidth;   //screen width      (in characters)
    uint32_t screenheight;  //screen height     (in characters)
    uint32_t pos;           //screen position   (in characters)
    color8b_t color;        //8 bit printing color
    void init_printer(){
        charpointer = (char*)0xb8000;
        pos = 0;
        screenwidth = 80;
        screenheight = 24;
        color = 0x7;    //dark white
        printf("------ printer test line ------\n");
        color = 0xf;    //light white:default color
    }
    void rollscreen(){
        uint32_t s = (screenheight-1)*screenwidth;
        uint32_t s2 = screenheight*screenwidth;
        for(uint32_t i = 0;i<s;i++){
            *(charpointer+(i<<1))= *(charpointer+((i+screenwidth)<<1));
            *(charpointer+(i<<1)+1)= *(charpointer+((i+screenwidth)<<1)+1);
        }
        for(uint32_t i = s;i<s2;i++){
            *(charpointer+(i<<1))= *(charpointer+((i+screenwidth)<<1));
            *(charpointer+(i<<1)+1)= *(charpointer+((i+screenwidth)<<1)+1);
        }
        pos-=screenwidth;
    }
    void putchar(char c){
        switch (c)
        {
        /* '\n' is equaling to '\r\n' here*/
        //new line
        case '\n':
            pos+=screenwidth;
        //return to the starting position of a line
        case '\r':
            pos-=(pos%screenwidth);
            break;
        //backspace
        case '\b':
            *(charpointer+(pos<<1))=' ';
            --pos;
            break;
        //tab character
        case '\t':
            pos=pos-(pos%4)+4;
            break;
        default:
            *(charpointer+(pos<<1)) = c;
            *(charpointer+(pos<<1)+1) = color;
            ++pos;
            break;
        }
        //arrived the end of the screen
        if(pos>=(screenheight*screenwidth))rollscreen();
    }
    void putstr(const char *strp){
        char* str = (char*)strp;    //convert a const pointer to a pointer
        while(*str!=STRING_END){
            putchar(*str);
            str++;                  //move to next
        }
    }
    extern "C" void printf(const char* format...){
        va_list ls;
        va_start(ls,format);
        char* str = (char*)format;
        while(*str!=STRING_END){
            if(*str=='%'){          //formatting char
                str++;
                if(*str=='s'){      //'%s' is replaced by a string
                    putstr(va_arg(ls,const char*));
                    str++;
                }
                else if(*str==STRING_END){ // '%' at the end of a string
                    putchar('%');
                }
                else{               //incomplete '%' flag
                    putchar('%');
                    putchar(*str);
                    str++;
                }
            }
            else{
                putchar(*str);      //put other chars normally
                str++;
            }
        }
    }
}