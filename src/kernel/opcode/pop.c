#include "kernel/opcodes.h"

/**
 * @brief Opcode POP
 *
 * Command:                 POP
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_pop_func(uint64_t *params) {
    if( stack_is_empty( stack ) ) {
        FATAL_DESC_ERROR("Cannot pop from empty stack!");
    }

#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64, params[0]);
#endif

    minx_registers_find_register(params[0])->value = *((uint64_t*)stackpop(stack));
}
