#include "kernel/opcodes.h"

/*
 * Command:                 LBSH
 * Parameters:              1: register-address
 * Affects Program Pointer: NO
 *
 *
 */
void minx_opc_lbsh_func(uint64_t *params) {
#ifdef DEBUGGING
    EXPLAIN_OPCODE("R%"PRIu64, params[0]);
#endif

    Register *r = minx_registers_find_register(params[0]);
    r->value = r->value<<8;
}
