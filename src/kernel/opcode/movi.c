#include "kernel/opcodes.h"

/**
 * @brief Opcode MOVI
 *
 * Command:                 MOVI
 * Parameters:              2: register-address value
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_movi_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "R%"PRIu64" <- %#010"PRIx64, 
            "R%"PRIu64" <- %"PRIu64, 
            params[0], params[1]);
#endif 

    minx_registers_find_register(params[0])->value = params[1];
}
