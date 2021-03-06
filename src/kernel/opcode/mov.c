#include "kernel/opcodes.h"

/**
 * @brief Opcode MOV 
 *
 * Command:                 MOV 
 * Parameters:              2: register-adress register-address
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_mov_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "R%"PRIu64" <- R%"PRIu64" (%#010"PRIx64")",
            "R%"PRIu64" <- R%"PRIu64" (%"PRIu64")",
                params[0], 
                params[1],
                minx_registers_find_register(params[1])->value); 
#endif 

    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);

    r1->value = r2->value;
}

