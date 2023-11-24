/**
 * @file pagealloc.h
 * @brief the index of default physical page allocators
*/
#ifndef _MEMORY_ALLOC_PAGEALLOC_H
#define _MEMORY_ALLOC_PAGEALLOC_H
#include "physical_alloc.h"

/**
 * @brief default 32-bit physical page allocator
 * @brief defined in @ref pagealloc32.c
*/
extern struct physical_allocator *phyical_alloc32;

#endif //_MEMORY_ALLOC_PAGEALLOC_H