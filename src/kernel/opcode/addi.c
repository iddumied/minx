#include "kernel/opcodes.h"

/*
 * Command:                 ADDI
 * Parameters:              2: register-address, value
 * Affects Program Pointer: NO
 *
 * Result in akku
 */
void opc_addi_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg %"PRIu64" + %"PRIu64, params[0], params[1]);
#endif

    minx_registers_find_register(akku_register_number)->value = 
        minx_registers_find_register(params[0])->value + params[1];

}
