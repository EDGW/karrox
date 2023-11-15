#ifndef _ARCH_ARCH_H
#define _ARCH_ARCH_H
/*      A public interface of arches        */
/*      Any boots should implement this interface       */
namespace arch{
    /*      get a string which describes the boot       */
    const char* getbootinfo();
}
#endif //_ARCH_ARCH_H