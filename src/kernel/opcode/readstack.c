#include "kernel/opcodes.h"

/*
 * Command:                 READSTACK
 * Parameters:              0:
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
void minx_opc_readstack_func (uint64_t* params) {
#ifdef DEBUGGING
    /* nothing here */
#endif

    Register *akku = minx_registers_find_register(akku_register_number);

    akku->value = (uint64_t)stack->first->data;
}
