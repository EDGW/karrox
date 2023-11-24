#include "physical_alloc.h"
#include "boot/memory.h"
#include "rb_tree.h"
#include "stdint.h"

#define PAGE_SIZE       ((size_t)4096)
#define MAX_MEM_SIZE    (~(size_t)0)
#define MAX_PAGE_COUNT  (MAX_MEM_SIZE / PAGE_SIZE)

static size_t total_page_count;
static size_t free_page_count;
static struct rb_tree       page_map;
static struct rb_tree_node  page_nodes[MAX_PAGE_COUNT];

static int use_page(const size_t idx){
    page_nodes[idx].key=idx;
    rb_delete(&page_map,&page_nodes[idx]);
}

static int alloc_page(physical_page* pg){
    if(rb_is_null(page_map.root_node))return 0;
    else{
        *pg = page_map.root_node->key;
        use_page(*pg);
    }
}

static size_t get_page_size(){
    return PAGE_SIZE;
}

static size_t get_page_count(){
    return total_page_count;
}

static size_t get_free_page_count(){
    return free_page_count;
}

static void init(){
    total_page_count = get_physical_page_count();
    free_page_count = total_page_count;
    //init tree nodes
    init_rb_tree(&page_map);
    for(int i = 0;i<total_page_count;i++){
        page_nodes[i].key=i;
        rb_insert(&page_map,&page_nodes[i]);
    }
    //allocate lower 64MiB pages
    for(int i = 0;i<(64*1024*1024)/PAGE_SIZE;i++){
        use_page(i);
    }
}

static struct physical_allocator phyical_alloc32_struct =
{
    .init               = init,
    .get_page_count     = get_page_count,
    .get_free_page_count= get_free_page_count,
    .get_page_size      = get_page_size,
    .alloc_page         = alloc_page,
};

struct physical_allocator *phyical_alloc32 = &phyical_alloc32_struct;
