#include "memory/paging.h"

.section .bootstrap_stack, "aw", @nobits
	.align 16
	stack_bottom:
	.skip 16384 # 16 KiB
	stack_top:

.section .multiboot.text,"a"
	/**
	 * @brief create some basic high-address pages for kernel, or kernel would not be loaded correctly
	 * map lower 32MiB 8+8(repeated) Page Directory Entries and 8 Page Tables
	 */
	.type create_basic_pages, @function
	create_basic_pages:
		movl $BOOT_KERNEL_PT_POS,%ebx
		movl $(BOOT_KERNEL_PT_POS+0x1000),%eax
		movl $8,%ecx
		movl %eax,%ds:(%ebx)
		.crt_pde:
			orl  $0x3,%eax
			movl %eax,%ds:(%ebx)
			movl %eax,%ds:0xC00(%ebx) //Item #d & #768+d (upper 3GiB)
			addl $0x4,%ebx		//Add 4Bytes
			addl $0x1000,%eax 	//Add 4KiB
			loop .crt_pde
		mov $0x0,%eax
		mov $0x0,%edx
		movl $8,%ecx
		.crt_tb:
			call create_page_table
			add $0x1000,%eax
			add $(4*1024*1024),%edx
			loop .crt_tb
		mov $BOOT_KERNEL_PT_POS,%eax
		mov %eax,%cr3		//cr3, page table offset
		mov %cr0,%eax
		or  $0x80000001,%eax//cr0 PG enabled
		mov %eax,%cr0
		ret

	/**
	 * @brief create a page table
	 * @param %eax table offset
	 * @param %edx map addr
	 * param stack space was disposed by function
	 */
	.type create_basic_pages, @function
	create_page_table:
		push %eax	// backup registers
		push %edx
		push %ecx
		mov $(BOOT_KERNEL_PT_POS+0x1000),%ebx
		addl %eax,%ebx
		mov $1024,%ecx
		.crt_entry:	// create a page table entry
			orl $0b100000011,%edx
			movl %edx,%ds:(%ebx) 
			add $0x4,%ebx
			add $0x1000,%edx
			loop .crt_entry
		pop %ecx	// recover registers
		pop %edx
		pop %eax
		ret

	/**
	 * @brief the entry point of the kernel
	 * @brief init stack,create basic page for high-address kernel virtual basic and jump to kernel
	 */
	.global _start
	.type _start, @function
	_start:
		/*the CPU is in 32-bit protected mod when using GRUB to boot the system*/
		mov $stack_top, %esp	//set stack top
		pushl %ebx				//push multiboot tag addr as main arguments
		call create_basic_pages
		call x86_boot_main		//call main
		cli
	1:	hlt
		jmp 1b

	.size _start, . - _start
