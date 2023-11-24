#include "physical_alloc.h"
#include "pagealloc.h"

struct physical_allocator *allocator;

void init_physical_page_allocators(){
    allocator = phyical_alloc32;
    allocator->init();
}

