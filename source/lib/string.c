/**
 * @file string.c
 * @brief implements some basic operations of string
*/
#include "stdint.h"
#include "string.h"
void memset(void* dst__,int8_t value,size_t siz)
{
    uint8_t* dst = (uint8_t*)dst__;
    while(siz--){
        *dst++=value;
    }
}