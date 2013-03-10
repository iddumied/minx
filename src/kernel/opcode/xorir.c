#include "kernel/opcodes.h"

/**
 * @brief Opcode XORIR
 *
 * Command:                 XORIR
 * Parameters:              2: register-address, value 
 * Affects Program Pointer: NO
 *
 * Result in akku
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_xorir_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" ^ %"PRIu64, params[0], params[1]);
#endif

    Register *r1 = minx_registers_find_register(params[0]);

    r1->value = r1->value ^ params[1];
}
