#include "kernel/opcodes.h"

/*
 * Command:                 MOVI
 * Parameters:              2: register-address value
 * Affects Program Pointer: NO
 *
 *
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
