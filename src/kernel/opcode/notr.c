#include "kernel/opcodes.h"

/*
 * Command:                 NOTR
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 * Result in same register
 */
void minx_opc_notr_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("reg: %"PRIu64, params[0]);
#endif 

    minx_registers_find_register(params[0])->value = ! minx_registers_find_register(params[0])->value;
}
