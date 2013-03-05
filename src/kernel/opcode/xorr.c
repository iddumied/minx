#include "kernel/opcodes.h"

/**
 * @brief Opcode XORR
 *
 * Command:                 XORR
 * Parameters:              2: register-address, register-address 
 * Affects Program Pointer: NO
 *
 * Result in akku
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_xorr_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" ^ R%"PRIu64, params[0], params[1]);
#endif

    Register *r1 = minx_registers_find_register(params[0]);
    Register *r2 = minx_registers_find_register(params[1]);

    r1->value = r1->value ^ r2->value;
}
