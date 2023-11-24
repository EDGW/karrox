/**
 * @file  main.c
 * @brief These code are where the boot starts from.
 * the main function @ref x86_boot_main is called by the boot initializer in 'boot.S'
 *  
*/
#include "boot/multiboot2.h"
#include "memory/memory.h"
#include "interrupt/interrupt.h"
#include "bootinfo.h"
#include "kernel/kernel.h"

/**
 * @brief a temporary printer which is only used to print some message before the kernel is loaded.
 * @brief all the printed strings will be cleared by the kernel printer.
*/
#pragma region EARLY_PRINTER
static char* early_print_pointer = (char*)0xb8000;
static uint32_t printer_pos = 0;
static char charset[] = "0123456789ABCDEF";
static inline void early_putchar(char c){
    switch(c){
        case '\n':
            printer_pos+=160;
        case '\r':
            printer_pos-=(uint32_t)printer_pos%160;
            break;
        default:
            *(early_print_pointer+printer_pos) = c;
            *(early_print_pointer+printer_pos+1) = 0xf;
            printer_pos+=2;
            break;
    }
}
static inline void early_putstr(char* c){
    while(*c!='\0'){
        early_putchar(*c);
        c++;
    }
}
static inline void early_putint(uint32_t value,uint8_t radix){
    uint32_t base = 1;
    while(value/base > radix){
        base*=radix;
    }
    while(base){
        early_putchar(charset[value/base]);
        value%=base;
        base/=radix;
    }
}
#pragma endregion //EARLY_PRINTER

/**
 * @brief the basic information about this PC
*/
struct bootinfo boot_info;

/**
 * @brief resolve a multiboot tag and save it to the boot info.
*/
void read_boot_tag(struct multiboot_tag* tag){
    if(tag->type == MULTIBOOT_TAG_TYPE_BASIC_MEMINFO){
        struct multiboot_tag_basic_meminfo* restag = (struct multiboot_tag_basic_meminfo*)tag;
        boot_info.memory.lower_mem_size = restag->mem_lower;
        boot_info.memory.upper_mem_size = restag->mem_upper;
    }
}

/**
 * @brief read all the tags that the multiboot loader delieverd.
*/
void read_tags(struct multiboot_info* tagptr){
    struct multiboot_tag *tag = tagptr->tags;
    uint32_t sz = tagptr->total_size;
    while((void*)tag - (void*)tagptr + tag->size <= sz){ //tagptr will have at lease 1 tags.
        read_boot_tag(tag);
        uint32_t tgps = (uint32_t)tag + tag->size;
        if(tgps%8!=0){
            tgps = tgps-(tgps%8)+8;
        }           //align to 8-bytes
        tag = (struct multiboot_tag*)tgps;
    }
}

/**
 * @brief stop initializing and print error message because of the deadly error.
*/
void dead(char *msg){
    early_putstr("[FATAL]unable to initialize kernel:");
    early_putstr(msg);
    early_putstr("\ntry to check your hardware and reboot later.\n");
    while(1)asm volatile("hlt");
}

/**
 * @brief read boot info and check it.
*/
void init_boot_info(struct multiboot_info* tagptr){
    read_tags(tagptr);
    
    /* check boot info      */
    if(boot_info.memory.lower_mem_size==0){
        dead("unable to detect memory size.");
    }
}

struct bootinfo* get_bootinfo(){//extern "bootinfo.c"
    return &boot_info;
}

/**
 * @brief the main boot function, which is called by the boot initializer in 'boot.S'
 * @param tagp a pointer to where the multiboot header tag stores, 
 *             in stack, this argument is pushed earlier than the returning addr is pushed (%esp+8);
*/
int x86_boot_main(struct multiboot_info* tagptr){
    init_boot_info(tagptr);
    init_memory();
    init_intr();
    kernel_main();
    return 0;
}