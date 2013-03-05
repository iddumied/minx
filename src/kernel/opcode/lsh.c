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
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg %"PRIu64, params[0]);
#endif

    Register *r = minx_registers_find_register(params[0]);
    r->value = r->value<<1;
}
