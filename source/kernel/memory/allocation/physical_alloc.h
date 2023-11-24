/**
 * @file physical_alloc.h
 * @brief defines a general interface of physicall page allocator &
 * @brief provides basic initialization and management functions of physical page allocators
*/
#ifndef KERNEL_PHYSICAL_ALLOC_H
#define KERNEL_PHYSICAL_ALLOC_H
#include "stdint.h"
typedef size_t physical_page;

struct physical_allocator{
    void (*init)();
    size_t (*get_page_count)();
    size_t (*get_free_page_count)();
    size_t (*get_page_size)();
    int (*alloc_page)(physical_page* page);
    int (*free_page)(physical_page* page);
};

/**
 * @brief initialize physical page allocators
*/
void init_physical_page_allocators();
#endif //KERNEL_PHYSICAL_ALLOC_H