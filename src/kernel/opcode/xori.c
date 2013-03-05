#include "kernel/opcodes.h"

/**
 * @brief Opcode XORI
 *
 * Command:                 XORI
 * Parameters:              2: register-address, value 
 * Affects Program Pointer: NO
 *
 * Result in akku
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_xori_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64" ^ %"PRIu64, params[0], params[1]);
#endif

    Register *r1 = minx_registers_find_register(akku_register_number);
    Register *r2 = minx_registers_find_register(params[0]);

    r1->value = r2->value ^ params[1];
}