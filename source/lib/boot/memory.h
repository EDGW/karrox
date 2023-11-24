#ifndef _LIB_MEMORY_H
#define _LIB_MEMORY_H
#include "stdint.h"
#ifndef ASM_FILE
/**
 * @brief get the count of physical pages
*/
size_t get_physical_page_count();
#endif //ASM_FILE
#endif  //_LIB_MEMORY_H
