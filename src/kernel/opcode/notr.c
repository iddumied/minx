#include "kernel/opcodes.h"

/**
 * @brief Opcode NOTR
 *
 * Command:                 NOTR
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 * Result in same register
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_notr_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg: %"PRIu64, params[0]);
#endif 

    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);
    
    r1->value = !r2->value;
}
