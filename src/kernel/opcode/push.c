#include "kernel/opcodes.h"

/**
 * @brief Opcode PUSH
 *
 * Command:                 PUSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_push_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "R%"PRIu64" (%#010"PRIx64")",
            "R%"PRIu64" (%"PRIu64")",
            params[0], minx_registers_find_register(params[0])->value);
#endif

    stackpush(stack, &(minx_registers_find_register(params[0])->value), VALUE_SIZE) ;
}
