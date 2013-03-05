#include "kernel/opcodes.h"

/**
 * @brief Opcode READSTACK
 *
 * Command:                 READSTACK
 * Parameters:              0:
 * Affects Program Pointer: NO
 *
 * Result in akku
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_readstack_func (uint64_t* params) {
#ifdef DEBUGGING
    /* nothing here */
#endif

    Register *akku = minx_registers_find_register(akku_register_number);

    akku->value = (uint64_t)stack->first->data;
}
