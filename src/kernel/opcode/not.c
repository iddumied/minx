#include "kernel/opcodes.h"

/**
 * @brief Opcode NOT 
 *
 * Command:                 NOT 
 * Parameters:              1: register-address 
 * Affects Program Pointer: NO
 *
 * Result in minx_registers_find_register(akku_register_number)->value
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_not_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg: %"PRIu64, params[1]);
#endif 

    Register *r1 = minx_registers_find_register(akku_register_number);
    Register *r2 = minx_registers_find_register(params[1]);

    r1->value = !r2->value;
}
