/**
 * @file head.S
 * @brief the header of multiboot2 loader.
*/

#include "boot/multiboot2.h"
.section .multiboot.data
.align 8
.set magic  ,MULTIBOOT2_HEADER_MAGIC                //header magic number
.set arch   ,MULTIBOOT_ARCHITECTURE_I386            //arch info
.set header_sz,header_end - header_start                //header length
.set header_chksum , -(magic + arch + header_sz)    //header checksum (magic+arch+length+checksum)=0
header_start:
    /* header */
    .long magic
    .long arch
    .long header_sz
    .long header_chksum

.set tag_reqinfo    ,MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
.set optional       ,MULTIBOOT_HEADER_TAG_OPTIONAL
.set tag_sz         ,tag_end - tag_start
.set tagtype_meminfo,MULTIBOOT_TAG_TYPE_BASIC_MEMINFO
    /* tags */
    tag_start:
    .word tag_reqinfo
    .word optional
    .long tag_sz
    .long tagtype_meminfo

    tag_end:
.section .multiboot.data
.align 8
.word 0,0
.long 8
header_end: