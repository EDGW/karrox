
/**      
 * @file   arch.h
 * @brief A public interface of arches        
 * @version 1.0.0.0
 * Any boots should implement this interface 'io' interface.
 */

#ifndef _ARCH_ARCH_H
#define _ARCH_ARCH_H
#include "systeminfo.h"
/**
 * @brief the default namespace of this interface
 */
namespace arch{
    /**
     * @brief get a string which describes the boot       
     */
    const char* getbootinfo();
    /*      initialize some basic information       */
    void init();

    /**
     *@brief  a pointer to systeminfo struct which describes the basic information about the system
     *use a pointer instead of the value itself because the struct is too large to be put in stack.
     */
    system_info* getsysteminfo();
}
#endif //_ARCH_ARCH_H