#include "kernel/opcodes.h"

/*
 * Command:                 MOV 
 * Parameters:              2: register-adress register-address
 * Affects Program Pointer: NO
 *
 *
 */
static void opc_mov_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "R%"PRIu64" <- R%"PRIu64" (%#010"PRIx64")",
            "R%"PRIu64" <- R%"PRIu64" (%"PRIu64")",
                params[0], 
                params[1],
                minx_registers_find_register(params[1])->value); 
#endif 

    minx_registers_find_register(params[0])->value = minx_registers_find_register(params[1])->value;
}

