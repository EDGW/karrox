## the Developing Environment
### Environment
To make sure the following programs are installed on your computer
 - bochs
 - grub
 - gcc (12.2.0 or above)
 - make

It's advisable to use Visual Studio Code or other advanced editors.

### Run and Debug

The descriptions of Project

#### Makefile Targets
 -  **`make (all)`**      
    the default option
    compile and link all sources and generate an image file

 -  **`make run`**        
    run 'make all' and start bochs emulator

 -  **`make mount`**      
    mount the iso file(readonly) to the 'mountediso' folder under the project path

 -  **`make umount`**
    umount the iso file
#### Makefile Variables
- **`$(OBJ_DIR)`**
    The name of a object file directory which is used to store the unlinked object file.

    By default the value is `.obj`,which means the compiler reads the source `boot/boot.cpp` and places the output file in `boot/.obj/boot.o`


 - **`$(CC)`**
    The compiler of C C++ and Assembly files

    By default the value is `gcc`


 - **`$(CC_ARGS_32)`**
    Default 32bit-only compiling options

    By default the value is `-m32 -Werror`


 - **`$(CC_ARGS_COMMON)`**
    Default compiling options 

    By default the value is same to `$(CC_ARGS_32)`


 - **`$(ARCH)`**
    The arch of the project

    By default the value is `x86`

    In machines of differenct architectures, instrument set, registers, memory styles, interruptions and so many other things are different, and karrox is a cross-architecture operating system and is maintaining a good compability to other platforms.

    The arch-based sources are placed in `kernel/boot/arch/{arch_type}/`, and `kernel/boot/arch/arch.h` defines a set of interfaces that makes the connection between the arch-based part and the cross-platform part.

 - **`$(KERNEL_FILE)`**
    The output filename of kernel

    By default the value is `$(OBJ_DIR)/kernel.sys`