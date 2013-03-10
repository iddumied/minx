#include "kernel/opcodes.h"

/**
 * @brief Opcode ORIR
 *
 * Command:                 ORIR
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 *
 *
 * @param params Pointer to parameters for this opcode
 */
void minx_opc_orir_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE_WITH_HEXF(
            "R%"PRIu64" (%#010"PRIx64") | %#010"PRIx64, 
            "R%"PRIu64" (%"PRIu64") | %"PRIu64, 
            params[0], minx_registers_find_register(params[0])->value,
            params[1]);
#endif 

    minx_registers_find_register(params[0])->value |= params[1];
}
