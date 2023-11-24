#include "memory/allocation/physical_alloc.h"
int kernel_main(void){
    uint8_t* c = (uint8_t*)0xb8000;
    *(c)='Z';
    *(c+1)=0xf;
    init_physical_page_allocators();
    *(c)='A';
    *(c+1)=0xf;
    while(1);
    return 0;
}