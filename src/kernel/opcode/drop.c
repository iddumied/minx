#include "kernel/opcodes.h"

/**
 * @brief Opcode DROP
 *
 * Command:                 DROP
 * Parameters:              0
 * Affects Program Pointer: NO
 *
 * Remove element from stack
 *
 * @param params Pointer to parameters for this opcode
 *
 */
void minx_opc_drop_func(uint64_t *params) {
#ifdef DEBUGGING
    /*no explaining here, nothing to explain*/
#endif
    if( stack_is_empty( stack ) ) {
        FATAL_DESC_ERROR("Cannot drop from empty stack!");
    }

    stackpop(stack);
}
