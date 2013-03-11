#include "kernel/opcodes.h"

/**
 * @brief Opcode LSH
 *
 * Command:                 LSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_lsh_func(uint64_t *params) {
    Register *r = minx_registers_find_register(params[0]);

#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "R%"PRIu64" (%#010"PRIx64")", 
            "R%"PRIu64" (%"PRIu64")", 
            params[0], r->value);
#endif

    r->value = r->value<<1;
}
