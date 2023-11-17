
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/*
The linker script specifies _start as the entry point to the kernel and the
bootloader will jump to this position once the kernel has been loaded. It
doesn't make sense to return from this function as the bootloader is gone.
*/
.section .text
.global _start
.type _start, @function
_start:
	/*the CPU is in 32-bit protected mod when using GRUB to boot the system*/
	mov $stack_top, %esp	//set stack top
	pushl %ebx				//push multiboot tag addr as main arguments
	call boot_x86_main		//call main
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
