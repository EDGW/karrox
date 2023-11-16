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
    void reser_printer_color(color8b_t clr){
        color=0xf;
    }
    void set_printer_color(color8b_t clr){
        color &= 0b11111000u;
        color |=clr;
    }
    void set_printer_background(color8b_t clr){
        color &= 0b10001111u;
        color |=clr<<4;
    }
    void set_printer_highlight(bool on){
        if(on)color |= 0b00001000u;
        else color &= 0b11110111u;
    }
    void set_printer_flashing(bool on){
        if(on)color |= 0b10000000u;
        else color &= 0b01111111u;
    }
    void init_printer(){
        charpointer = (char*)0xb8000;
        pos = 0;
        screenwidth = 80;
        screenheight = 24;
        color = 0;
        set_printer_color(CLR_WHITE);
        printf("------ printer test line ------\n");
        set_printer_highlight(ON);
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
    extern "C" void asm_putstr(char* str){
        const char* ptr = (const char*)str;
        putstr(ptr);
    }
    char radixnum[] = {'0','1','2','3','4','5','6','7','8','9',
                        'A','B','C','D','E','F','G',
                        'H','I','J','K','L','M','N',
                        'O','P','Q','R','S','T',
                        'U','V','W','X','Y','Z',
                        'a','b','c','d','e','f',
                        'g','h','i','j','k','l','m','n',
                        'o','p','q','r','s','t',
                        'u','v','w','x','y','z'};
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
                else if(*str=='d'){      //'%s' is replaced by a string
                    putint((int32_t)va_arg(ls,int32_t));
                    str++;
                }
                else if(*str=='o'){      //'%s' is replaced by a string
                    putinto((int32_t)va_arg(ls,int32_t));
                    str++;
                }
                else if(*str=='b'){      //'%s' is replaced by a string
                    putintb((int32_t)va_arg(ls,int32_t));
                    str++;
                }
                else if(*str=='x' || *str=='X'){      //'%s' is replaced by a string
                    putinth((int32_t)va_arg(ls,int32_t));
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